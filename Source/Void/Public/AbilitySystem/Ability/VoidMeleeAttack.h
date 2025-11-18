// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/VoidDamageAbilityBase.h"
#include "VoidMeleeAttack.generated.h"

class AVoidWeapon;

/**
 * 
 */
UCLASS()
class VOID_API UVoidMeleeAttack : public UVoidDamageAbilityBase
{
	GENERATED_BODY()

public:


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float HeavyDamageMultiplier;

	UFUNCTION(BlueprintCallable)
	void SetWeaponEffectSpec(AVoidWeapon* Weapon, bool bHeavyAttack);
	
};
