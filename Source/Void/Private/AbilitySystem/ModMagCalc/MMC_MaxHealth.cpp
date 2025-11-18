// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/VoidAttributeSet.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	LevelDef.AttributeToCapture = UVoidAttributeSet::GetLevelAttribute();
	LevelDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LevelDef.bSnapshot = false;

	AddedMaxHealthDef.AttributeToCapture = UVoidAttributeSet::GetAddedMaxHealthAttribute();
	AddedMaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AddedMaxHealthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(LevelDef);
	RelevantAttributesToCapture.Add(AddedMaxHealthDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather Tags From Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float TempLevel = 1.f;
	GetCapturedAttributeMagnitude(LevelDef, Spec, EvaluationParameters, TempLevel);
	int32 Level = FMath::RoundToInt32<float>(TempLevel);
	Level = FMath::Max<int32>(Level, 1);
	
	float AddedMaxHealth = 0.f;
	GetCapturedAttributeMagnitude(AddedMaxHealthDef, Spec, EvaluationParameters, AddedMaxHealth);
	AddedMaxHealth = FMath::Max<float>(0.f, AddedMaxHealth);

	return 80.f + (20.f * Level) + AddedMaxHealth;

}
