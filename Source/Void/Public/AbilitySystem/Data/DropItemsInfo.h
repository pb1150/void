// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DropItemsInfo.generated.h"

class AVoidItem;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	AttributeEffectItem UMETA(DisplayName = "AttributeEffectItem"),
	AbilityEffectItem   UMETA(DisplayName = "AbilityEffectItem"),
	Weapon				UMETA(DisplayName = "Weapon")
};


USTRUCT(BlueprintType)
struct FDropItemsDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInfo")
	TSubclassOf<AVoidItem> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInfo")
	EItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInfo")
	float DropProbability;
};


/**
 * 
 */
UCLASS()
class VOID_API UDropItemsInfo : public UDataAsset
{
	GENERATED_BODY()

};
