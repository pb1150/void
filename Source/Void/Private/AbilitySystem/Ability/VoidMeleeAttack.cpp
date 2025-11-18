// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/VoidMeleeAttack.h"
#include "Item/Weapon/VoidWeapon.h"
#include "AbilitySystemComponent.h"
#include "VoidAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"



void UVoidMeleeAttack::SetWeaponEffectSpec(AVoidWeapon* Weapon, bool bHeavyAttack)
{
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FVoidGameplayEffectContext* CustomContext = new FVoidGameplayEffectContext();
	CustomContext->AddInstigator(SourceASC->GetAvatarActor(), SourceASC->GetOwnerActor());
	CustomContext->SetAbility(this);
	CustomContext->SetIsHeavyAttack(bHeavyAttack);

	FGameplayEffectContextHandle EffectContextHandle = FGameplayEffectContextHandle(CustomContext);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
	if (bHeavyAttack)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FVoidGameplayTags::Get().Damage, HeavyDamageMultiplier);
	}
	else
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FVoidGameplayTags::Get().Damage, PlayerDamageMultiplier);
	}
	
	Weapon->DamageEffectSpecHandle = SpecHandle;
}
