// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputKeyDisplayData.generated.h"

USTRUCT(BlueprintType)
struct FKeyDisplayPair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    FName KeyName;  

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    FName DisplayName;
};

/**
 * 
 */
UCLASS()
class VOID_API UInputKeyDisplayData : public UDataAsset
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    TArray<FKeyDisplayPair> KeyDisplayMappings;

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input")
    FName GetDisplayNameForKey(const FName& InKey) const;
	
};
