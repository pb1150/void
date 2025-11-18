// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_Damage.h"
#include "AbilitySystem/VoidAttributeSet.h"


UMMC_Damage::UMMC_Damage()
{
	LevelDef.AttributeToCapture = UVoidAttributeSet::GetLevelAttribute();
	LevelDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LevelDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(LevelDef);
}

float UMMC_Damage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	return  10.f + (Level * 5.f);
}
