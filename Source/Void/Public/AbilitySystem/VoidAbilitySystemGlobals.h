// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "VoidAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API UVoidAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()


	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
