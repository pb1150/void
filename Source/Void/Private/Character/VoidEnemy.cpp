// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VoidEnemy.h"
#include "Void/Void.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "AbilitySystem/VoidAttributeSet.h"
#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widget/VoidUserWidget.h"
#include "VoidGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/VoidAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AVoidEnemy::AVoidEnemy()
{
	Tags.Add("Enemy");

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetVisibility(false);

	AbilitySystemComponent = CreateDefaultSubobject<UVoidAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UVoidAttributeSet>("AttributeSet");

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetDrawSize(FVector2D(456, 32));

}

void AVoidEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	VoidAIController = Cast<AVoidAIController>(NewController);
	VoidAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	VoidAIController->RunBehaviorTree(BehaviorTree);
	VoidAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	VoidAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), false);
	VoidAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsSpawning"), true);
}

void AVoidEnemy::HighLightActor()
{
	check(M_HighLight);
	if (GetMesh()) GetMesh()->SetOverlayMaterial(M_HighLight);
}

void AVoidEnemy::UnHighLightActor()
{
	if (GetMesh()) GetMesh()->SetOverlayMaterial(nullptr);
}

UAnimMontage* AVoidEnemy::GetMeleeAttackMontage_Implementation()
{
	return MeleeMontage;
}

int32 AVoidEnemy::GetMeleeMontageSectionCount_Implementation()
{
	return MeleeMontageSectionCount;
}

void AVoidEnemy::SetCombatTarget_Implementation(AActor* InTarget)
{
	CombatTarget = InTarget;
}

AActor* AVoidEnemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

FVector AVoidEnemy::GetRightCombatSocketLocation_Implementation()
{
	if (RightCombatSocket.IsValid() && GetMesh())
	{
		return GetMesh()->GetSocketLocation(RightCombatSocket);
	}
	return FVector();
}

FVector AVoidEnemy::GetLeftCombatSocketLocation_Implementation()
{
	if (LeftCombatSocket.IsValid() && GetMesh())
	{
		return GetMesh()->GetSocketLocation(LeftCombatSocket);
	}
	return FVector();
}

FVector AVoidEnemy::GetSpawnActorLocation_Implementation()
{
	return FVector();
}

UAnimMontage* AVoidEnemy::GetSpawnActorMontage_Implementation()
{
	return SpawnActorMontage;
}

ECharacterClass AVoidEnemy::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

UParticleSystem* AVoidEnemy::GetSpawnParticleSystem_Implementation()
{
	if (SpawnParticleSystem)
	{
		return SpawnParticleSystem;
	}
	return nullptr;
}

FVector AVoidEnemy::GetSpawnParticleSystemLocation_Implementation()
{
	return SpawnParticleSystemLocation;
}

UAnimMontage* AVoidEnemy::GetSpawnAnimMontage_Implementation()
{
	if (SpawnMontage)
	{
		return SpawnMontage;
	}
	return nullptr;
}

void AVoidEnemy::EndSpawnEnemy_Implementation()
{
	if (VoidAIController)
	{
		IsSpawning = false;
		OnSpawnFinishDelegate.Broadcast(IsSpawning);
		VoidAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsSpawning"), false);
	}
}

int32 AVoidEnemy::GetPlayerLevel()
{
	return Level;
}

void AVoidEnemy::Die()
{
	Super::Die();
	HealthBar->SetVisibility(false);
	if (VoidAIController && VoidAIController->GetBlackboardComponent())
	{
		VoidAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);
	}
	if (EnemyDeadSound && this)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EnemyDeadSound, GetActorLocation());
	}
}

FVector AVoidEnemy::GetDamageTextSocketLocation()
{
	if (GetMesh())
	{
		return GetMesh()->GetSocketLocation(FName("DamageTextSocket"));
	}
	return FVector();
}

void AVoidEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitAbilityActorInfo();
	UVoidBlueprintFunctionLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass, Level);

	if (HealthBar)
	{
		HealthBar->SetWorldScale3D(FVector(HealthBarScale));
		UVoidUserWidget* VoidWidegt = Cast<UVoidUserWidget>(HealthBar->GetUserWidgetObject());
		if (VoidWidegt)
		{
			VoidWidegt->SetWidgetController(this);
		}
	}

	UVoidAttributeSet* VoidAS = CastChecked<UVoidAttributeSet>(AttributeSet);
	if (VoidAS)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VoidAS->GetHealthAttribute()).AddUObject(this, &AVoidEnemy::OnHealthChange);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VoidAS->GetMaxHealthAttribute()).AddUObject(this, &AVoidEnemy::OnMaxHealthChange);
		AbilitySystemComponent->RegisterGameplayTagEvent(FVoidGameplayTags::Get().Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AVoidEnemy::HitReactTagChanged);
		//AbilitySystemComponent->AbilityActivatedCallbacks()

		OnHealthChangedDelegate.Broadcast(VoidAS->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(VoidAS->GetMaxHealth());
	}

}

void AVoidEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HealthBar)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			FVector CamLoc;
			FRotator CamRot;
			PC->GetPlayerViewPoint(CamLoc, CamRot);

			const FVector From = HealthBar->GetComponentLocation();
			const FVector To = CamLoc;
			const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(From, To);
			HealthBar->SetWorldRotation(LookAt);
		}

	}
}

void AVoidEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	UVoidAbilitySystemComponent* VoidASC = Cast<UVoidAbilitySystemComponent>(AbilitySystemComponent);
	VoidASC->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void AVoidEnemy::InitializeDefaultAttributes()
{
	UVoidBlueprintFunctionLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AVoidEnemy::HitReactTagChanged(const FGameplayTag Tag, int32 Count)
{
	bHitReacting = Count > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;

	if (VoidAIController && VoidAIController->GetBlackboardComponent())
	{
		VoidAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}


void AVoidEnemy::OnHealthChange(const FOnAttributeChangeData& Data)
{
	OnHealthChangedDelegate.Broadcast(Data.NewValue);
}

void AVoidEnemy::OnMaxHealthChange(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
}


