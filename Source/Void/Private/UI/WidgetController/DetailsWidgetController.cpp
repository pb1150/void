// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/DetailsWidgetController.h"
#include "AbilitySystem/VoidAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/Data/AttributeEffectActorsInfo.h"
#include "VoidGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"


UDetailsWidgetController::UDetailsWidgetController()
{
}

void UDetailsWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	check(AttributeEffectActorInfo);
	UVoidAttributeSet* AS = CastChecked<UVoidAttributeSet>(AttributeSet);
	UVoidAbilitySystemComponent* VoidASC = CastChecked<UVoidAbilitySystemComponent>(AbilitySystemComponent);
	if (AS == nullptr || VoidASC == nullptr) return;

	for (TTuple<FGameplayTag, FGameplayAttribute>& Pair : AS->TagstoAttributes)
	{
		FVoidAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
		Info.AttributeValue = Pair.Value.GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}

	if (VoidASC->PickedupActors.IsEmpty()) return;
	int i = 0;
	for (FAttributeEffectActors& Pair : VoidASC->PickedupActors)
	{
		FAttributeActorInfo* ActorInfo = AttributeEffectActorInfo->FindAttributeActorInfoByTag(Pair.ActorTag);
		if (ActorInfo != nullptr)
		{
			ActorInfo->Count = Pair.Count;
			ActorInfo->SlotNum = i;
			AttributeEffectActorInfoDelegate.Broadcast(*ActorInfo);
			i++;
		}

	}

}

void UDetailsWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	check(AttributeEffectActorInfo);

	UVoidAttributeSet* AS = CastChecked<UVoidAttributeSet>(AttributeSet);
	UVoidAbilitySystemComponent* VoidASC = CastChecked<UVoidAbilitySystemComponent>(AbilitySystemComponent);
	if (AS == nullptr || VoidASC == nullptr) return;

	for (TTuple<FGameplayTag, FGameplayAttribute>& Pair : AS->TagstoAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data) 
			{
				FVoidAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
				Info.AttributeValue = Data.NewValue;
				AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}

	VoidASC->EffectActorInfo.AddLambda(
		[this, VoidASC](const FAttributeEffectActors& Actor) {

			FAttributeActorInfo* ActorInfo = AttributeEffectActorInfo->FindAttributeActorInfoByTag(Actor.ActorTag);
			ActorInfo->Count = Actor.Count;
			ActorInfo->SlotNum = Actor.SlotNum;
			AttributeEffectActorInfoDelegate.Broadcast(*ActorInfo);
		}
	);

}
