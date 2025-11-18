// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/VoidDamageAbilityBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "VoidGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "Interaction/EnemyInterface.h"

void UVoidDamageAbilityBase::CauseDamage(AActor* TargetActor)
{
	const float FinalMultiplier = GetDamageMultiplierFromCharacterClass();
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FVoidGameplayTags::Get().Damage, FinalMultiplier);

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

float UVoidDamageAbilityBase::GetDamageMultiplierFromCharacterClass()
{
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->Implements<UEnemyInterface>()) return 1.f;
	ECharacterClass CharacterClass = IEnemyInterface::Execute_GetCharacterClass(GetAvatarActorFromActorInfo());
	float* DamageMultiplier = CharacterClassToDamageMultiplier.Find(CharacterClass);
	float FinalMultiplier = (DamageMultiplier != nullptr) ? *DamageMultiplier : 1.f;
	return FinalMultiplier;
}
