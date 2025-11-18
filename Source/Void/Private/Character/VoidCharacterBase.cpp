// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VoidCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"



AVoidCharacterBase::AVoidCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* AVoidCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


FVector AVoidCharacterBase::GetProjectileSocketLocation_1_Implementation()
{
	if (GetMesh())
	{
		return GetMesh()->GetSocketLocation(FName("R_ProjectilesSocket1"));
	}
	return FVector();
}

USkeletalMeshComponent* AVoidCharacterBase::GetSkeletalMesh_Implementation()
{
	return GetMesh();
}

void AVoidCharacterBase::Die()
{
	bDead = true;

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	GetMesh()->SetOverlayMaterial(nullptr);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AVoidCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

void AVoidCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void AVoidCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVoidCharacterBase::Jump()
{
	Super::Jump();
}

void AVoidCharacterBase::StopJumping()
{
	Super::StopJumping();
}

void AVoidCharacterBase::InitAbilityActorInfo()
{

}

void AVoidCharacterBase::Dissolve()
{

}

void AVoidCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AVoidCharacterBase::InitializeDefaultAttributes()
{
	ApplyEffectToSelf(DefaultLevel, 1.f);
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
}

void AVoidCharacterBase::InitializePassiveBuff()
{
	ApplyEffectToSelf(PassiveBuff, 1.f);
}





