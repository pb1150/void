// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/VoidAbilityBase.h"
#include "VoidTheWorld.generated.h"


/**
 * 
 */
UCLASS()
class VOID_API UVoidTheWorld : public UVoidAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void BeginDestroy() override;

	void QuitTheWorld();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TObjectPtr<UMaterialInterface> M_TheWorld;  

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	FName ScalarParameterName = FName();

	UPROPERTY(EditDefaultsOnly, Category = "InterSpeed")
	float InterpSpeed = 10.f;

private:

	FTimerHandle TimerHandle_Interp;
	FTimerHandle TimerHandle_End;

	float TargetTimeDilation = 4.0f;
	float CurrentTimeDilation = 1.0f;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MID_TheWorld; 

	UPROPERTY()
	TObjectPtr<APostProcessVolume> PostProcessVolumeRef;
};
