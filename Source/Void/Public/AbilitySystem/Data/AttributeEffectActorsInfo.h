// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeEffectActorsInfo.generated.h"


UENUM(BlueprintType)
enum class EActorQuality : uint8
{
	Common UMETA(DisplayName = "Common"),
	Rare   UMETA(DisplayName = "Rare"),
	Epic   UMETA(DisplayName = "Epic")
};



USTRUCT(BlueprintType)
struct FAttributeActorInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorInfo")
	FGameplayTag ActorTag = FGameplayTag();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ActorInfo")
	int32 Count = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorInfo")
	EActorQuality ActorQuality = EActorQuality::Common;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorInfo")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorInfo")
	FText Name = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorInfo")
	FText ActorDescription = FText();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ActorInfo")
	int32 SlotNum = 0;
};



/**
 * 
 */
UCLASS()
class VOID_API UAttributeEffectActorsInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttributeActorInfo> AttributeActorInformation;

	FAttributeActorInfo* FindAttributeActorInfoByTag(const FGameplayTag& Tag, bool bLogNotFound = false);
};
