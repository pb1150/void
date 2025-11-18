// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "VoidGameplayTags.h"
#include "Item/Weapon/VoidWeapon.h"
#include "WeaponInfo.generated.h"

USTRUCT(BlueprintType)
struct FVoidWeaponInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AVoidWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* LightAttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HeavyAttackSound;
	
};


/**
 * 
 */
UCLASS()
class VOID_API UWeaponInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FVoidWeaponInfo> VoidWeaponInformation;

	FVoidWeaponInfo FindWeaponInfoByTag(FGameplayTag WeaponTag);
	
};
