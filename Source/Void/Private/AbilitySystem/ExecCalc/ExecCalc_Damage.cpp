// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/VoidAttributeSet.h"
#include "VoidGameplayTags.h"
#include "VoidAbilityTypes.h"

struct VoidDamageStatic
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);

	VoidDamageStatic()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVoidAttributeSet, Damage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVoidAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVoidAttributeSet, CriticalHitMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVoidAttributeSet, DamageReduction, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UVoidAttributeSet, BlockChance, Target, false);
	}
};

static const VoidDamageStatic& DamageStatic()
{
	static VoidDamageStatic DStatic;
	return DStatic;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatic().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitMultiplierDef);
	RelevantAttributesToCapture.Add(DamageStatic().DamageReductionDef);
	RelevantAttributesToCapture.Add(DamageStatic().BlockChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;

	FVoidGameplayEffectContext* VoidEffectContext = static_cast<FVoidGameplayEffectContext*>(Spec.GetContext().Get());

	//Get Damage Set by Caller Magnitude

	float DamageMultiplier = 0.f;
	DamageMultiplier = Spec.GetSetByCallerMagnitude(FVoidGameplayTags::Get().Damage);

	//Get Attributes from Target and Source 
	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DamageDef, EvaluationParams, Damage);
	Damage = FMath::Max<float>(0.f, Damage);
	Damage *= DamageMultiplier;

	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().BlockChanceDef, EvaluationParams, BlockChance);
	BlockChance = FMath::Max<float>(0.f, BlockChance);
	BlockChance = FMath::Min<float>(0.5f, BlockChance); //BlockChance最多叠加到50%

	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitChanceDef, EvaluationParams, CriticalHitChance);
	CriticalHitChance = FMath::Max<float>(0.f, CriticalHitChance);

	float CriticalHitMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().CriticalHitMultiplierDef, EvaluationParams, CriticalHitMultiplier);
	CriticalHitMultiplier = FMath::Max<float>(0.f, CriticalHitMultiplier);

	float DamageReduction = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatic().DamageReductionDef, EvaluationParams, DamageReduction);
	DamageReduction = FMath::Max<float>(0.f, DamageReduction);
	

	const bool bBlocked = FMath::RandRange(0.f, 100.f) < (BlockChance * 100.f);
	if (bBlocked)
	{
		Damage *= 0.5f;
	}

	const bool bCriticalhit = FMath::RandRange(0.f, 100.f) < (CriticalHitChance * 100.f);
	if (bCriticalhit && !bBlocked)
	{
		Damage *= CriticalHitMultiplier;
	}

	VoidEffectContext->SetIsBlockedHit(bBlocked);
	VoidEffectContext->SetIsCriticalHit(bCriticalhit);

	Damage -= DamageReduction;
	Damage = FMath::Max<float>(1.f, Damage);
	FGameplayModifierEvaluatedData EvaluatedData(UVoidAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage);

	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
