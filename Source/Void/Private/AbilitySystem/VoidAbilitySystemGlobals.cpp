// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/VoidAbilitySystemGlobals.h"
#include "VoidAbilityTypes.h"

FGameplayEffectContext* UVoidAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FVoidGameplayEffectContext();
}
