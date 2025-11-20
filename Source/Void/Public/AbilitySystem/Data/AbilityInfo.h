// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FVoidAbilityInfo
{
	GENERATED_BODY()

	FORCEINLINE bool operator==(const FVoidAbilityInfo& Other) const
	{
		return AbilityTag == Other.AbilityTag;  
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<UTexture2D> AbilityIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<UMaterialInterface> Background = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfo")
	TObjectPtr<UMaterialInterface> Progress = nullptr;

};


/**
 * 
 */
UCLASS()
class VOID_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInfomation")
	TArray<FVoidAbilityInfo> AbilityInfomation;

	FVoidAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
};
