// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "AbilitySystem/VoidAttributeSet.h"
#include "AbilitySystem/Ability/VoidAbilityBase.h"
#include "VoidGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"



UVoidAbilitySystemComponent::UVoidAbilitySystemComponent()
{
	PickedupActors.Empty();
}

void UVoidAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UVoidAbilitySystemComponent::EffectApplied);
}

FAttributeEffectActors UVoidAbilitySystemComponent::FindAttributeEffectActorsByTag(FGameplayTag Tag)
{
	for (const FAttributeEffectActors& Actor : PickedupActors)
	{
		if (Actor.ActorTag.MatchesTagExact(Tag))
		{
			return Actor;
		}
	}
	return FAttributeEffectActors();
}

void UVoidAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, GetSet<UVoidAttributeSet>()->GetLevel());
		if (const UVoidAbilityBase* VoidAbility = Cast<UVoidAbilityBase>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(VoidAbility->StartupInputAction);
			GiveAbility(AbilitySpec);
			
			FAbilityMessage AbilityMessage(VoidAbility->AbilityTags.First(), VoidAbility->StartupInputAction);
			AbilityChangedDelegate.Broadcast(AbilityMessage);
		}
	}
}

void UVoidAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, GetSet<UVoidAttributeSet>()->GetLevel());
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UVoidAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(Spec);

			if (!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
		}

	}


}



void UVoidAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{

			AbilitySpecInputReleased(Spec);

			// 修复：手动触发 InputReleased 事件
			if (Spec.IsActive())
			{
				TArray<UGameplayAbility*> Instances = Spec.GetAbilityInstances();
				if (Instances.Num() > 0)
				{
					const FGameplayAbilityActivationInfo& ActivationInfo = Instances.Last()->GetCurrentActivationInfoRef();
					InvokeReplicatedEvent(
						EAbilityGenericReplicatedEvent::InputReleased,
						Spec.Handle,
						ActivationInfo.GetActivationPredictionKey()
					);
				}
			}
		}
	}
}

void UVoidAbilitySystemComponent::ChangeAbility(TSubclassOf<UVoidAbilityBase> NewAbility)
{
	FGameplayAbilitySpec NewAbilitySpec = FGameplayAbilitySpec(NewAbility, GetSet<UVoidAttributeSet>()->GetLevel());
	UVoidAbilityBase* VoidAbility = Cast<UVoidAbilityBase>(NewAbilitySpec.Ability);
	if (VoidAbility == nullptr) return;

	if (!GetActivatableAbilities().IsEmpty())
	{
		TArray<FGameplayAbilitySpecHandle> HandlesToClear;

		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(VoidAbility->StartupInputAction))
			{
				HandlesToClear.Add(AbilitySpec.Handle);
			}
		}

		for (const FGameplayAbilitySpecHandle& Handle : HandlesToClear)
		{
			CancelAbilityHandle(Handle);
			ClearAbility(Handle);
		}
	}

	NewAbilitySpec.DynamicAbilityTags.AddTag(VoidAbility->StartupInputAction);
	GiveAbility(NewAbilitySpec);
	FAbilityMessage AbilityMessage(VoidAbility->AbilityTags.First(), VoidAbility->StartupInputAction);
	AbilityChangedDelegate.Broadcast(AbilityMessage);
}

void UVoidAbilitySystemComponent::BroadcastAbilityInfo()
{
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		const UVoidAbilityBase* VoidAbility = Cast<UVoidAbilityBase>(Spec.Ability);
		if (VoidAbility)
		{
			FAbilityMessage AbilityMessage(VoidAbility->AbilityTags.First(), VoidAbility->StartupInputAction);
			AbilityChangedDelegate.Broadcast(AbilityMessage);
		}
	}
}

void UVoidAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);

	for (const FGameplayTag& Tag : TagContainer.GetGameplayTagArray())
	{
	
		if (PickedupActors.IsEmpty())
		{
			FAttributeEffectActors NewActor(Tag, 1, 0);
			PickedupActors.Add(NewActor);
			EffectActorInfo.Broadcast(NewActor);
			continue;
		}

		bool bFound = false;

		for (FAttributeEffectActors& Pair : PickedupActors)
		{
			if (Tag.MatchesTagExact(Pair.ActorTag))
			{
				Pair.Count++;
				FAttributeEffectActors Updated(Pair.ActorTag, Pair.Count, Pair.SlotNum);
				EffectActorInfo.Broadcast(Updated);
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			int32 NewSlot = PickedupActors.Num();
			FAttributeEffectActors NewActor(Tag, 1, NewSlot);
			PickedupActors.Add(NewActor);
			EffectActorInfo.Broadcast(NewActor);
		}
	}

	/*float Damage = EffectSpec.GetSetByCallerMagnitude(FVoidGameplayTags::Get().Damage);
	if (Damage == 0.f) return;

	UObject* SourceObj = EffectSpec.GetEffectContext().GetSourceObject();
	if (AActor* SourceActor = Cast<AActor>(SourceObj))
	{
		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
		if (SourceASC)
		{
			const FGameplayTag LightSlashTag = FVoidGameplayTags::Get();
			const int32 LightSlashStacks = SourceASC->GetTagCount(LightSlashTag);


			UE_LOG(LogTemp, Log, TEXT("Source %s has %d LightSlashOnHit stacks"), *SourceActor->GetName(), LightSlashStacks);
		}
        

	}*/
}

