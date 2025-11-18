// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "VoidProjectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UParticleSystem;

UCLASS()
class VOID_API AVoidProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AVoidProjectile();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

protected:

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	/* 飞行物设置：子弹，飞刀等*/
	UFUNCTION()
	void OnBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* End 飞行物设置：子弹，飞刀等*/


	/* 飞行物设置：雷电，龙卷风等*/
	UFUNCTION(BlueprintCallable)
	void StartTimerDamage();

	void DetectionAndApplyDamageByEffectSpecHandle();
	/* End 飞行物设置：雷电，龙卷风等*/


	UPROPERTY(EditDefaultsOnly, Category = "VoidProjectile")
	float LifeSpan = 15.f;


	/* 飞行物设置：雷电，龙卷风等*/

	UPROPERTY(EditDefaultsOnly, Category = "VoidProjectile")
	float TimerInterval = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "VoidProjectile")
	int32 MaxLoopCount = 3;

	UPROPERTY(VisibleAnywhere, Category = "VoidProjectile")
	int32 CurrentLoopCount = 0;

	UPROPERTY(EditDefaultsOnly, Category = "VoidProjectile")
	float DamageRadius = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "VoidProjectile")
	bool bForce = false;

	UPROPERTY(EditDefaultsOnly, Category = "VoidProjectile")
	float ForceStrength = 400.f;

	/* End 飞行物设置：雷电，龙卷风等*/


	/* 飞行物设置：子弹，飞刀等*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VoidProjectile|bool")
	bool bSimulatePhysics = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VoidProjectile|bool")
	bool CanHitEnemy = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VoidProjectile|bool")
	bool CanHitImpluse = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VoidProjectile|bool")
	bool CanUseBoxCollision = true;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ImpactSound;

	/* End 飞行物设置：子弹，飞刀等*/

private:
	FTimerHandle ProjectileTimer;

};
