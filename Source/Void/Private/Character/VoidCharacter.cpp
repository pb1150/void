// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VoidCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/VoidPlayerState.h"
#include "Player/VoidPlayerController.h"
#include "AbilitySystemComponent.h"
#include "UI/HUD/VoidHUD.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "AbilitySystem/VoidAttributeSet.h"
#include "VoidGameplayTags.h"
#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"
#include "Item/Weapon/VoidMeleeWeapon.h"
#include "Interaction/ItemInterface.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Game/VoidGameInstance.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "CableComponent.h"

AVoidCharacter::AVoidCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh(), TEXT("neck_01")); 
	CameraBoom->TargetArmLength = 0.f;                   
	CameraBoom->bUsePawnControlRotation = true;           

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(CameraBoom);

	CableTargetLocation = CreateDefaultSubobject<USceneComponent>("CableTargetLocation");
	CableTargetLocation->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("spine_03"));

	CableComponent = CreateDefaultSubobject<UCableComponent>("CableComponent");
	CableComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("spine_03"));
	CableComponent->SetVisibility(false);
	

	/** Intialize WeaponMapInfo */
	WeaponInfoMap.Add(FVoidGameplayTags::Get().Weapon_None, EWeaponType::None);
	WeaponInfoMap.Add(FVoidGameplayTags::Get().Weapon_Melee_BaseballBat, EWeaponType::BaseballBat);
	WeaponInfoMap.Add(FVoidGameplayTags::Get().Weapon_Melee_BlueSword, EWeaponType::Sword);
	WeaponInfoMap.Add(FVoidGameplayTags::Get().Weapon_Range_Gun, EWeaponType::Gun);

	/** End Intialize WeaponMapInfo */

}

void AVoidCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	GetMesh()->HideBoneByName(FName("head"), EPhysBodyOp::PBO_None);
	EquipWeapon(HeldWeaponTag);
}

void AVoidCharacter::EquipWeapon(const FGameplayTag& Tag)
{
	UVoidBlueprintFunctionLibrary::SpawnAndEquipWeapon(this, HeldWeapon, Tag, GetMesh());
	HeldWeaponTag = Tag;
	if (HeldWeapon)
	{
		HeldWeapon->SetOwner(this);
		Cast<UVoidAbilitySystemComponent>(AbilitySystemComponent)->ChangeAbility(HeldWeapon->WeaponAbility);
	}
	OnWeaponTypeChangedDelegate.Broadcast(FindWeaponTypeByTag(Tag));
}

void AVoidCharacter::EquipWeaponByTag(const FGameplayTag& Tag)
{
	EquipWeapon(Tag);
}

void AVoidCharacter::AddToXp_Implementation(int32 InXP)
{
	AVoidPlayerState* VoidPlayerState = GetPlayerState<AVoidPlayerState>();
	check(VoidPlayerState);
	VoidPlayerState->AddToXP(InXP);
}

void AVoidCharacter::LevelUp_Implementation()
{
	if (IsValid(LevelUpSound))
	{
		UGameplayStatics::PlaySound2D(this, LevelUpSound);
	}
}

int32 AVoidCharacter::GetXp_Implementation() const
{
	AVoidPlayerState* VoidPlayerState = GetPlayerState<AVoidPlayerState>();
	check(VoidPlayerState);
	return VoidPlayerState->GetXP();
}

int32 AVoidCharacter::FindLevelForXp_Implementation(int32 InXP) const
{
	AVoidPlayerState* VoidPlayerState = GetPlayerState<AVoidPlayerState>();
	check(VoidPlayerState);
	return VoidPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

void AVoidCharacter::AddToPlayerLevel_Implementation(int32 PlayerLevel)
{
	AVoidPlayerState* VoidPlayerState = GetPlayerState<AVoidPlayerState>();
	check(VoidPlayerState);
	VoidPlayerState->AddToPlayerLevel(PlayerLevel);
}

void AVoidCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	
}

void AVoidCharacter::OnRep_PlayerState()
{
	InitAbilityActorInfo();
}

void AVoidCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bIsSprinting ? SprintFOV : WalkFOV;
	float CurrentFOV = FirstPersonCameraComponent->FieldOfView;
	float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, FOVInterpSpeed);
	FirstPersonCameraComponent->SetFieldOfView(NewFOV);
}

void AVoidCharacter::InitAbilityActorInfo()
{
	AVoidPlayerState* VoidPlayerState = GetPlayerState<AVoidPlayerState>();
	check(VoidPlayerState);
	VoidPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(VoidPlayerState, this);
	AbilitySystemComponent = VoidPlayerState->GetAbilitySystemComponent();
	AttributeSet = VoidPlayerState->GetAttributeSet();
	UVoidAbilitySystemComponent* VoidASC = Cast<UVoidAbilitySystemComponent>(AbilitySystemComponent);
	VoidASC->AbilityActorInfoSet();

	AVoidPlayerController* PlayerController = GetController<AVoidPlayerController>();
	if (PlayerController)
	{
		AVoidHUD* VoidHUD = PlayerController->GetHUD<AVoidHUD>();
		if (VoidHUD)
		{
			VoidHUD->InitOverlay(PlayerController, VoidPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	LoadProgress();
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVoidAttributeSet::GetMoveSpeedMultiplierAttribute()).AddUObject(this, &AVoidCharacter::MoveSpeedMultiplierChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UVoidAttributeSet::GetSprintSpeedMultiplierAttribute()).AddUObject(this, &AVoidCharacter::SprintSpeedMultiplierChanged);
	UVoidAttributeSet* VoidAS = Cast<UVoidAttributeSet>(AttributeSet);
	CurrentWalkSpeed = BaseWalkSpeed * UVoidAttributeSet::GetMoveSpeedMultiplierAttribute().GetNumericValue(VoidAS);
	CurrentSprintSpeed = BaseSprintSpeed * UVoidAttributeSet::GetSprintSpeedMultiplierAttribute().GetNumericValue(VoidAS) * UVoidAttributeSet::GetMoveSpeedMultiplierAttribute().GetNumericValue(VoidAS);
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? CurrentSprintSpeed : CurrentWalkSpeed;
}

void AVoidCharacter::TempSaveCharacterProgress_Implementation()
{
	UVoidBlueprintFunctionLibrary::TempSaveCharacterProgress(this, GetPlayerState(), AbilitySystemComponent, AttributeSet);
}

USceneComponent* AVoidCharacter::GetCableTargetComponent_Implementation()
{
	return CableTargetLocation;
}

UCableComponent* AVoidCharacter::GetCableComponent_Implementation()
{
	return CableComponent;
}

void AVoidCharacter::LoadProgress()
{
	UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(GetGameInstance());
	if (VoidGameInstance == nullptr) return;

	UVoidAbilitySystemComponent* VoidASC = CastChecked<UVoidAbilitySystemComponent>(AbilitySystemComponent);

	if (VoidGameInstance->IsInit)
	{
		InitializeDefaultAttributes();
		VoidASC->AddCharacterAbilities(StartupAbilities);
	}
	else
	{
		AVoidPlayerState* VoidPlayerState = GetPlayerState<AVoidPlayerState>();

		VoidPlayerState->SetPlayerLevel(VoidGameInstance->PlayerLevel);
		VoidPlayerState->SetXP(VoidGameInstance->XP);

		HeldWeaponTag = VoidGameInstance->WeaponTag;
		VoidASC->PickedupActors = VoidGameInstance->PickedupActors;
		UVoidBlueprintFunctionLibrary::LoadCharacterAttribute(this, AbilitySystemComponent, DefaultLevel_SetByCaller, DefaultPrimaryAttributes, DefaultSecondaryAttributes_SetByCaller);
		
		TArray<TSubclassOf<UGameplayAbility>> OwnedAbilities;
		UVoidBlueprintFunctionLibrary::LoadCharacterAbilties(this, OwnedAbilities);
		VoidASC->AddCharacterAbilities(OwnedAbilities);
	}
	InitializePassiveBuff();
	VoidASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void AVoidCharacter::MoveSpeedMultiplierChanged(const FOnAttributeChangeData& Data)
{
	UVoidAttributeSet* VoidAS = Cast<UVoidAttributeSet>(AttributeSet);
	CurrentWalkSpeed = BaseWalkSpeed * Data.NewValue;
	CurrentSprintSpeed = BaseSprintSpeed * Data.NewValue * UVoidAttributeSet::GetSprintSpeedMultiplierAttribute().GetNumericValue(VoidAS);
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentSprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	}
}

void AVoidCharacter::SprintSpeedMultiplierChanged(const FOnAttributeChangeData& Data)
{
	CurrentSprintSpeed = BaseSprintSpeed * Data.NewValue;
	if (bIsSprinting) GetCharacterMovement()->MaxWalkSpeed = CurrentSprintSpeed;
}

EWeaponType AVoidCharacter::FindWeaponTypeByTag(const FGameplayTag& WeaponTag)
{
	for (const auto& Pair : WeaponInfoMap)
	{
		if (Pair.Key.MatchesTagExact(WeaponTag))
		{
			return Pair.Value;
		}
	}
	return EWeaponType();
}


FVector AVoidCharacter::GetRifleBulletSocketLocation_1_Implementation()
{
	if (HeldWeapon && Cast<IItemInterface>(HeldWeapon))
	{
		return Cast<IItemInterface>(HeldWeapon)->GetBulletSocketLocation();
	}
	return FVector();
}

FRotator AVoidCharacter::GetRifleBulletSocketRotation_1_Implementation()
{
	if (HeldWeapon && Cast<IItemInterface>(HeldWeapon))
	{
		return Cast<IItemInterface>(HeldWeapon)->GetBulletSocketRotation();
	}
	return FRotator();
}

int32 AVoidCharacter::GetPlayerLevel()
{
	AVoidPlayerState* VoidPlayerState = GetPlayerState<AVoidPlayerState>();
	if (VoidPlayerState)
	{
		return VoidPlayerState->GetPlayerLevel();
	}
	return 1.f;
}

void AVoidCharacter::Die()
{
	if (GetMesh()) GetMesh()->UnHideBoneByName(FName("head"));
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController && AbilitySystemComponent)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;

		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		PlayerController->SetInputMode(InputMode);
	}

	Super::Die();
}

bool AVoidCharacter::PlayerSprint()
{
	if (bIsSprinting)
	{
		// Switch to Walk
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	}
	else
	{
		// Switch to Sprint
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = CurrentSprintSpeed;
	}
	
	return bIsSprinting;
}

void AVoidCharacter::SwitchToWalk_Implementation()
{
	if (!bIsSprinting) return;
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	AVoidPlayerController* PlayerController = GetController<AVoidPlayerController>();
	PlayerController->OnSprintChangedDelegate.Broadcast(false);
}

AVoidWeapon* AVoidCharacter::GetActorHeldWeapon_Implementation()
{
	return HeldWeapon;
}

FGameplayTag AVoidCharacter::GetHeldWeaponTag_Implementation()
{
	return HeldWeaponTag;
}

AVoidMeleeWeapon* AVoidCharacter::GetActorHeldMeleeWeapon_Implementation()
{
	return Cast<AVoidMeleeWeapon>(HeldWeapon);
}




