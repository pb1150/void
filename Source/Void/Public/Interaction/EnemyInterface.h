// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemyInterface.generated.h"

class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VOID_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void HighLightActor() = 0;
	virtual void UnHighLightActor() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetMeleeAttackMontage();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetSpawnActorMontage();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetMeleeMontageSectionCount();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateMotionLocation(const FVector& Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetRightCombatSocketLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetLeftCombatSocketLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetSpawnActorLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetWarpingLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ECharacterClass GetCharacterClass();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UParticleSystem* GetSpawnParticleSystem();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetSpawnParticleSystemLocation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UAnimMontage* GetSpawnAnimMontage();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndSpawnEnemy();
};
