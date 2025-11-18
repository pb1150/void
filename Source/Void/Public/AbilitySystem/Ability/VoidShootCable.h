// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/VoidAbilityBase.h"
#include "VoidShootCable.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API UVoidShootCable : public UVoidAbilityBase
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void BeginDestroy() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PullSpeed = 2200.f;      // 拉动角色的速度

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float UpwardSpeed = 500.f;      // 角色最后的向上速度
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float StopTolerance = 120.f;   // 停止的距离 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ValidDistance = 2500.f; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CableAcceptableRadius = 60.f;  //Cable停止的距离

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CableSpeed = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxAbilityDuration = 5.f;


	FTimerHandle CableShootTimer;
	FTimerHandle CableRetractTimer;
	FTimerHandle CableForceEndTimer;
	float ElapsedDist = 0.f;

	void CableShootTick(USceneComponent* CableTarget, FVector TargetLocation, bool bHit);
	void CableRetractTick(USceneComponent* CableTarget, FVector TargetLocation, bool bHit, TWeakObjectPtr<class UCableComponent> CableWeak, TWeakObjectPtr<class ACharacter> CharacterWeak, float InitialDisToTarget);
	void StartCableRetract(USceneComponent* CableTarget, const FVector& TargetLocation, bool bHit);
	void ForceEndAbility(USceneComponent* CableTarget, UCableComponent* Cable);

};
