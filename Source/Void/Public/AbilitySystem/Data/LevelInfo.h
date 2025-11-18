// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	Moon UMETA(DisplayName = "Moon")
};


USTRUCT(BlueprintType)
struct FLevelDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelInfo")
	TSoftObjectPtr<UWorld> LevelRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelInfo")
	FName LevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelInfo")
	int32 MaxKilledEnemyCount = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelInfo")
	ELevelType LevelType;
};


/**
 * 
 */
UCLASS()
class VOID_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelInfo")
	TArray<FLevelDefaultInfo> LevelDefaultList;
	
};
