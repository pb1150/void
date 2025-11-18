// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/VoidEffectItem.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

TMap<EEffectZoneType, TMap<TWeakObjectPtr<AActor>, int32>> AVoidEffectItem::GlobalOverlapCounts;
TMap<EEffectZoneType, TMap<FActiveGameplayEffectHandle, TWeakObjectPtr<UAbilitySystemComponent>>> AVoidEffectItem::GlobalActiveEffectHandles;


AVoidEffectItem::AVoidEffectItem()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AVoidEffectItem::BeginPlay()
{
	Super::BeginPlay();
}

void AVoidEffectItem::OnOverlap(AActor* TargetActor)
{
	const bool bIsEnemy = TargetActor->ActorHasTag("Enemy");
	if (bIsEnemy && !bApplyEffectsToEnemy) return;
	if (bPlaySound)
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);
	}
	
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap) {
		if (EffectZoneType != EEffectZoneType::None)
		{
			TMap<TWeakObjectPtr<AActor>, int32>& CountMap = GlobalOverlapCounts.FindOrAdd(EffectZoneType);
			int32& Count = CountMap.FindOrAdd(TargetActor);
			Count++;

			if (Count == 1)
			{
				ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
			}
		}
		else
		{
			ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
		}	
	}

	if (TargetActor->ActorHasTag("Player") && DamageEffectSpecHandle.IsValid())
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		DamageEffectSpecHandle.Data.Reset();
	}


}

void AVoidEffectItem::EndOverlap(AActor* TargetActor)
{
	const bool bIsEnemy = TargetActor->ActorHasTag("Enemy");
	if (bIsEnemy && !bApplyEffectsToEnemy) return;


	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyEndOverLap) {
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyEndOverLap) {
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyEndOverLap) {
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}


	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap){
		
		if (EffectZoneType == EEffectZoneType::None) return;
		TMap<TWeakObjectPtr<AActor>, int32>* CountMapPtr = GlobalOverlapCounts.Find(EffectZoneType);
		if (!CountMapPtr) return;
		
		int32* CountPtr = CountMapPtr->Find(TargetActor);
		if (!CountPtr) return;

		(*CountPtr)--;

		if (*CountPtr <= 0)
		{
			CountMapPtr->Remove(TargetActor);

			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if (!IsValid(TargetASC)) return;

			TMap<FActiveGameplayEffectHandle, TWeakObjectPtr<UAbilitySystemComponent>>* HandleMapPtr = GlobalActiveEffectHandles.Find(EffectZoneType);
			if (!HandleMapPtr) return;

			TArray<FActiveGameplayEffectHandle> HandlesToRemove;

			for (TTuple<FActiveGameplayEffectHandle, TWeakObjectPtr<UAbilitySystemComponent>>& HandlePair : *HandleMapPtr)
			{
				if (TargetASC == HandlePair.Value)
				{
					TargetASC->RemoveActiveGameplayEffect(HandlePair.Key);
					HandlesToRemove.Add(HandlePair.Key);
				}
			}

			for (FActiveGameplayEffectHandle& Pair : HandlesToRemove)
			{
				HandleMapPtr->FindAndRemoveChecked(Pair);
			}
		}

	}
	

}


void AVoidEffectItem::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	check(GameplayEffectClass);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContext);
	FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	bool bIsInfinite = SpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap && EffectZoneType != EEffectZoneType::None)
	{
		GlobalActiveEffectHandles.FindOrAdd(EffectZoneType).Add(ActiveEffectHandle, TargetASC);
	}

	if (EffectZoneType == EEffectZoneType::None && bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

