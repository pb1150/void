// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "AbilitySystem/Data/DropItemsInfo.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class AVoidEnemy;


UENUM(BlueprintType)
enum class ECharacterQuality : uint8
{
	Common UMETA(DisplayName = "Common"),
	Elite   UMETA(DisplayName = "Elite"),
	Boss   UMETA(DisplayName = "Boss")
};

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	RedBuff,
	BlueBuff,
	BlackBuff,
	HolyElemental,
	Unknown
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	FScalableFloat XPReward = FScalableFloat();
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TArray<FDropItemsDefaultInfo> DropItemsDefaultInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TSubclassOf<AVoidEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	float SpawnProbability = 0.f; /* This is the percentage */

	UPROPERTY(EditDefaultsOnly, Category = "Character Defaults")
	TObjectPtr<UTexture2D> EnemyIcon;
};



/**
 * 
 */
UCLASS()
class VOID_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfomation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> LevelAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInfo")
	int32 MaxAttributeEffectItemNum = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInfo")
	int32 MaxAbilityEffectItemNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInfo")
	int32 MaxWeaponNum = 2;

	FCharacterClassDefaultInfo GetCharacterDefaultInfo(ECharacterClass CharacterClass);

};
