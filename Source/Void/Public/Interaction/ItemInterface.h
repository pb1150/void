// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VOID_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void DestroySelf();
	virtual void HighLightItem(float TriggerTime);
	virtual void UnHighLightItem();
	virtual FGameplayTag GetWeaponTag();
	virtual bool IsWeapon();
	virtual bool IsAbilityEffectActor();
	virtual void ApplyAbilitytoActor(AActor* OtherActor);
	virtual FVector GetBulletSocketLocation();
	virtual FRotator GetBulletSocketRotation();
};
