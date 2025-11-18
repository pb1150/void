// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_BlockChance.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "VoidGameplayTags.h"

UMMC_BlockChance::UMMC_BlockChance()
{
}

float UMMC_BlockChance::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayEffectContextHandle& ContextHandle = Spec.GetContext();
	UVoidAbilitySystemComponent* VoidASC = Cast<UVoidAbilitySystemComponent>(ContextHandle.GetInstigatorAbilitySystemComponent());
	FAttributeEffectActors Actor = VoidASC->FindAttributeEffectActorsByTag(FVoidGameplayTags::Get().Message_MoreDifficultTimes);
	int32 Count = Actor.Count + 1; //这里+1是因为这个GE还没有应用，数量还没有加上去

	const float CurrentBlockChance = (AddedBlockChance * Count) / (AddedBlockChance * Count + 1);

	return CurrentBlockChance;
}
