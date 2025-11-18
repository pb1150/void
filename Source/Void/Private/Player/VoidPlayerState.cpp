// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VoidPlayerState.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "AbilitySystem/VoidAttributeSet.h"
#include "Net/UnrealNetwork.h"

AVoidPlayerState::AVoidPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UVoidAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UVoidAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void AVoidPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AVoidPlayerState, Level);
	DOREPLIFETIME(AVoidPlayerState, XP);
}

UAbilitySystemComponent* AVoidPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AVoidPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AVoidPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AVoidPlayerState::AddToPlayerLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AVoidPlayerState::SetPlayerLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AVoidPlayerState::OnRep_Level(int OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AVoidPlayerState::OnRep_XP(int OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}
