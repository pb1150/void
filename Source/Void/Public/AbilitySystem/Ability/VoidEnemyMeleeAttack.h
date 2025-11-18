// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/VoidDamageAbilityBase.h"
#include "VoidEnemyMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API UVoidEnemyMeleeAttack : public UVoidDamageAbilityBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<ECharacterClass, float> CharcaterClassToMeleeAttackRadius;
	
	UFUNCTION(BlueprintPure)
	float GetMeleeAttackRadius();
};
