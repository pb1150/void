// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/VoidDamageAbilityBase.h"
#include "VoidEnemySpawnActor.generated.h"

class AVoidItem;

/**
 * 
 */
UCLASS()
class VOID_API UVoidEnemySpawnActor : public UVoidDamageAbilityBase
{
	GENERATED_BODY()
public:


protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AVoidItem> ActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ProjectileClass;

	UFUNCTION(BlueprintCallable)
	void SpawnActor();
	
	UFUNCTION(BlueprintCallable)
	void SpawnProjectileToCombatTarget();

};
