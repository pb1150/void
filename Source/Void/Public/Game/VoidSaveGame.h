// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "VoidSaveGame.generated.h"


USTRUCT(BlueprintType)
struct FPlayHistory
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 LastReachedLevel;

    UPROPERTY(BlueprintReadOnly)
    int32 PlayerLevel;

    UPROPERTY(BlueprintReadOnly)
    ECharacterClass CharacterClass; /* Killed by this */

    TArray<FAttributeEffectActors> PickupActors;

    FGameplayTagContainer PlayerAbilityTag;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    float PlayDuration; // √Î
};



/**
 * 
 */
UCLASS()
class VOID_API UVoidSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintReadWrite)
    TArray<FPlayHistory> HistoryList;
};
