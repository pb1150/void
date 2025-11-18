// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VoidPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interaction/PlayerInterface.h"
#include "Camera/VoidCameraManager.h"
#include "Input/VoidInputComponent.h"
#include "Input/VoidInputConfig.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "UI/Widget/DamageTextComponent.h"
#include "Interaction/CombatInterface.h"

AVoidPlayerController::AVoidPlayerController()
{
	bReplicates = true;
}

void AVoidPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	EnemyBoxTrace();
	ItemBoxTrace(DeltaTime);

}

void AVoidPlayerController::ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetCharacter);
	if (IsValid(TargetCharacter) && DamageTextComponentClass && CombatInterface)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		const float HorizontalOffsetRange = 30.f; 
		const float VerticalOffsetRange = 20.f;  

		FVector RandomOffset(
			FMath::RandRange(-HorizontalOffsetRange, HorizontalOffsetRange),
			FMath::RandRange(-HorizontalOffsetRange, HorizontalOffsetRange),
			FMath::RandRange(0.f, VerticalOffsetRange) 
		);

		FVector FinalLocation = CombatInterface->GetDamageTextSocketLocation() + RandomOffset;
		DamageText->SetWorldLocation(FinalLocation);

		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AVoidPlayerController::EnemyBoxTrace()
{
	FVector ViewLocation;
	FRotator ViewRotation;
	GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector Start = ViewLocation;
	FVector End = Start + ViewRotation.Vector() * 2500.f;

	FVector HalfSize(10.f, 10.f, 10.f);

	TArray<AActor*> ActorsToIgnore;
	if (GetPawn()) ActorsToIgnore.Add(GetPawn());

	FHitResult Hit;

	bool bHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		HalfSize,
		ViewRotation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,  
		Hit,
		true
	);

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(Hit.GetActor());
	if (LastActor != ThisActor)
	{
		if (ThisActor) ThisActor->HighLightActor();
		if (LastActor) LastActor->UnHighLightActor();
	}

}

void AVoidPlayerController::ItemBoxTrace(float DeltaTime)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FVector Start = ViewLocation;
	const FVector End = Start + ViewRotation.Vector() * 200.f;
	const FVector HalfSize(10.f, 10.f, 10.f);

	TArray<AActor*> ActorsToIgnore;
	if (GetPawn()) ActorsToIgnore.Add(GetPawn());

	FHitResult Hit;
	const bool bHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		HalfSize,
		ViewRotation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true
	);


	LastItem = ThisItem;
	ThisItem = bHit ? Cast<IItemInterface>(Hit.GetActor()) : nullptr;

	if (LastItem != ThisItem)
	{
		if (LastItem) LastItem->UnHighLightItem();
		SwitchWeaponTriggerTime = 0.f;
	}
	if (ThisItem)
	{
		if (bSwitchWeaponTiming)
		{
			SwitchWeaponTriggerTime += DeltaTime;
		}
		else
		{
			SwitchWeaponTriggerTime = 0.f;
		}
		ThisItem->HighLightItem(SwitchWeaponTriggerTime);
	}
	else
	{
		SwitchWeaponTriggerTime = 0.f;
	}
}

void AVoidPlayerController::PauseGameByButton()
{
	const bool bIsPaused = UGameplayStatics::IsGamePaused(this);
	UGameplayStatics::SetGamePaused(this, !bIsPaused);

	if (!bIsPaused)
	{
		OnPauseGame.Broadcast();
		bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
	else
	{
		OnUnPauseGame.Broadcast();
		bShowMouseCursor = false;
		FInputModeGameOnly InputModeData;
		InputModeData.SetConsumeCaptureMouseDown(true);
		SetInputMode(InputModeData);
	}
}

void AVoidPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(VoidContext);


	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(VoidContext, 0);
	}


	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameOnly InputModeData; 
	InputModeData.SetConsumeCaptureMouseDown(true); 
	SetInputMode(InputModeData);

}

void AVoidPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UVoidInputComponent* VoidInputComponent = CastChecked<UVoidInputComponent>(InputComponent);
	if (VoidInputComponent)
	{
		//Jumping
		VoidInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AVoidPlayerController::Jump);
		VoidInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AVoidPlayerController::StopJumping);

		//Look
		VoidInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVoidPlayerController::Look);

		//Move
		VoidInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVoidPlayerController::Move);
		VoidInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AVoidPlayerController::Sprint);

		//Show Details
		VoidInputComponent->BindAction(ShowDetailsAction, ETriggerEvent::Triggered, this, &AVoidPlayerController::OpenDetails);
		VoidInputComponent->BindAction(ShowDetailsAction, ETriggerEvent::Completed, this, &AVoidPlayerController::CloseDetails);

		VoidInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &AVoidPlayerController::TriggerSwitchWeapon);
		VoidInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Completed, this, &AVoidPlayerController::CompletedSwitchWeapon);

		//Pause Game
		VoidInputComponent->BindAction(PauseGameAction, ETriggerEvent::Started, this, &AVoidPlayerController::PauseGame);

		VoidInputComponent->BindAbilityActions(InputConfig, this, &AVoidPlayerController::AbilityInputTagPressed, &AVoidPlayerController::AbilityInputTagReleased, &AVoidPlayerController::AbilityInputTagHeld);
	}

}

void AVoidPlayerController::Move(const FInputActionValue& Value)
{	
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	APawn* ControlledPawn = GetPawn<APawn>();

	if (ControlledPawn)
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
	}
}

void AVoidPlayerController::Look(const FInputActionValue& Value)
{
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	APawn* ControlledPawn = GetPawn<APawn>();
	if (ControlledPawn)
	{
		
		ControlledPawn->AddControllerYawInput(LookAxisVector.X * 0.4);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y * 0.4);
	}

}

void AVoidPlayerController::Jump(const FInputActionValue& Value)
{
	ACharacter* ControlledPawn = GetPawn<ACharacter>();
	if (ControlledPawn)
	{
		ControlledPawn->Jump();
	}
}

void AVoidPlayerController::StopJumping(const FInputActionValue& Value)
{
	ACharacter* ControlledPawn = GetPawn<ACharacter>();
	if (ControlledPawn)
	{
		ControlledPawn->StopJumping();
	}
}

void AVoidPlayerController::Sprint(const FInputActionValue& Value)
{
	IPlayerInterface* ControlledPawn = GetPawn<IPlayerInterface>();
	if (ControlledPawn)
	{
		const bool bIsSprinting = ControlledPawn->PlayerSprint();
		OnSprintChangedDelegate.Broadcast(bIsSprinting);
	}
}

void AVoidPlayerController::OpenDetails(const FInputActionValue& Value)
{
	OnOpenDetails.Broadcast();
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways); // 锁定鼠标
	InputMode.SetHideCursorDuringCapture(false);                         // 不再隐藏鼠标
	SetInputMode(InputMode);

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void AVoidPlayerController::CloseDetails(const FInputActionValue& Value)
{
	OnCloseDetails.Broadcast();
	FInputModeGameOnly InputModeData;
	InputModeData.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputModeData);

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
}


void AVoidPlayerController::TriggerSwitchWeapon(const FInputActionValue& Value)
{
	bSwitchWeaponTiming = true;
	if (SwitchWeaponTriggerTime >= 1.02f)
	{
		IPlayerInterface* ControlledPawn = GetPawn<IPlayerInterface>();
		if (ControlledPawn && ThisItem && ThisItem->IsWeapon())
		{
			ControlledPawn->EquipWeaponByTag(ThisItem->GetWeaponTag());
			ThisItem->DestroySelf();
			ThisItem = nullptr;
		}
		if (ControlledPawn && ThisItem && ThisItem->IsAbilityEffectActor())
		{
			ThisItem->ApplyAbilitytoActor(GetPawn<ACharacter>());
		}
	}
}

void AVoidPlayerController::CompletedSwitchWeapon(const FInputActionValue& Value)
{
	bSwitchWeaponTiming = false;
	if (SwitchWeaponTriggerTime < 1.02f)
	{
		SwitchWeaponTriggerTime = 0.f;
	}
}

void AVoidPlayerController::PauseGame(const FInputActionValue& Value)
{
	PauseGameByButton();
}

void AVoidPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{

}

void AVoidPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetPawn() || ICombatInterface::Execute_IsDead(GetPawn())) return;
	if (GetVoidASC() == nullptr) return;
	GetVoidASC()->AbilityInputTagReleased(InputTag);
}

void AVoidPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetPawn() || ICombatInterface::Execute_IsDead(GetPawn())) return;
	if (GetVoidASC() == nullptr) return;
	GetVoidASC()->AbilityInputTagHeld(InputTag);
}

UVoidAbilitySystemComponent* AVoidPlayerController::GetVoidASC()
{
	if(VoidASC == nullptr)
	{
		VoidASC = Cast<UVoidAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return VoidASC;
}

