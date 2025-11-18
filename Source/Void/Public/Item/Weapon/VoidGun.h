// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/VoidWeapon.h"
#include "VoidGun.generated.h"

class USkeletalMeshComponent;

/**
 * 
 */
UCLASS()
class VOID_API AVoidGun : public AVoidWeapon
{
	GENERATED_BODY()


public:
	AVoidGun();

	/** Item Interface */
	virtual void HighLightItem(float TriggerTime) override;
	virtual void UnHighLightItem() override;
	virtual FVector GetBulletSocketLocation() override;
	virtual FRotator GetBulletSocketRotation() override;
	/** End Item Interface */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	FName BulletSocket_0 = FName();

	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	FName BulletSocket_1 = FName();

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> GunMesh;
	
};
