// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "VoidAbilitySystemComponent.generated.h"

class UVoidAbilityBase;
struct FAbilityMessage;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTag, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectActorInfo, const FAttributeEffectActors&);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityChanged, const FAbilityMessage&);

USTRUCT()
struct FAttributeEffectActors
{
	GENERATED_BODY()

	FGameplayTag ActorTag;
	int32 Count;
	int32 SlotNum;

	FAttributeEffectActors() : ActorTag(FGameplayTag()), Count(0), SlotNum(INDEX_NONE) {}
	
	FAttributeEffectActors(FGameplayTag InTag, int32 NewCount, int32 InSlotNum = INDEX_NONE)
		: ActorTag(InTag), Count(NewCount), SlotNum(InSlotNum) {
	}

	bool operator==(const FGameplayTag& OtherTag) const
	{
		return ActorTag.MatchesTagExact(OtherTag);
	}

	bool operator==(const FAttributeEffectActors& Other) const
	{
		return ActorTag.MatchesTagExact(Other.ActorTag);
	}
};

struct FAbilityMessage
{
	FAbilityMessage(FGameplayTag InAbilityTag, FGameplayTag InInputActionTag)
	{
		AbilityTag = InAbilityTag;
		InputActionTag = InInputActionTag;
	}
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag InputActionTag = FGameplayTag();
};



/**
 * 
 */
UCLASS()
class VOID_API UVoidAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UVoidAbilitySystemComponent();

	void AbilityActorInfoSet();

	FEffectAssetTag EffectAssetTags;
	FEffectActorInfo EffectActorInfo;
	FAbilityChanged AbilityChangedDelegate;

	TArray<FAttributeEffectActors> PickedupActors;

	FAttributeEffectActors FindAttributeEffectActorsByTag(FGameplayTag Tag);
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable)
	void ChangeAbility(TSubclassOf<UVoidAbilityBase> NewAbility);
	void BroadcastAbilityInfo();

protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
