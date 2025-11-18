// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/VoidItem.h"
#include "VoidGameplayTags.h"
#include "GameplayEffectTypes.h"
#include "VoidWeapon.generated.h"

class UCapsuleComponent;
class UWidgetComponent;
class UVoidAbilityBase;

/**
 * 
 */
UCLASS()
class VOID_API AVoidWeapon : public AVoidItem
{
	GENERATED_BODY()

public:
	AVoidWeapon();

	/** Item Interface */
	virtual void DestroySelf() override;
	virtual FGameplayTag GetWeaponTag() override;
	virtual bool IsWeapon() override;

	/** End Item Interface */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UVoidAbilityBase> WeaponAbility;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTag;

	void OnEquipWeapon();
	void OnUnEquipWeapon();
	bool bEquip = false;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditDefaultsOnly)
	float ImpulseStrength = 500.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	void AddHitImpulse(AActor* OtherActor);
	void AddCameraShake();
		
};
