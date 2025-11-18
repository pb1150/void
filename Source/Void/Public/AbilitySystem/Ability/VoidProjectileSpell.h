// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/VoidDamageAbilityBase.h"
#include "VoidProjectileSpell.generated.h"

class AVoidProjectile;

/**
 * 
 */
UCLASS()
class VOID_API UVoidProjectileSpell : public UVoidDamageAbilityBase
{
	GENERATED_BODY()

public:


protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AVoidProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SpawnRifleProjectile(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SpawnLightning();

	UFUNCTION(BlueprintCallable)
	void SpawnArea();

	UFUNCTION(BlueprintCallable)
	void SpawnTornado(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void SpawnBlackHole(const APlayerController* PlayerController);

	AVoidProjectile* SpawnActorByTransform(FVector SpawnLocation, FRotator SpawnRotation, bool CanHitReact);

private:

	UFUNCTION()
	void OnProjectileDestroyed(AActor* DestroyedActor);
};
