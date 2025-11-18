// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/VoidAbilityBase.h"
#include "AbilitySystem\Data\CharacterClassInfo.h"
#include "VoidDamageAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API UVoidDamageAbilityBase : public UVoidAbilityBase
{
	GENERATED_BODY()
	
	/* This is for Enemy */
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<ECharacterClass, float> CharacterClassToDamageMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float PlayerDamageMultiplier = 1.f;

	float GetDamageMultiplierFromCharacterClass();
};
