// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/VoidTheWorld.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "VoidGameplayTags.h"
#include "TimerManager.h"

void UVoidTheWorld::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;

	//  Create Dynamic Material Instance
	if (IsValid(M_TheWorld))
	{
		MID_TheWorld = UMaterialInstanceDynamic::Create(M_TheWorld, this);
		PostProcessVolumeRef = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));

		if (IsValid(PostProcessVolumeRef) && IsValid(MID_TheWorld))
		{
			FWeightedBlendable Blendable;
			Blendable.Object = MID_TheWorld;
			Blendable.Weight = 1.0f;
			PostProcessVolumeRef->Settings.WeightedBlendables.Array.Add(Blendable);
		}
	}

	UGameplayStatics::SetGlobalTimeDilation(Avatar->GetWorld(), 0.25f);
	CurrentTimeDilation = 1.f;

	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(TimerHandle_Interp);
		W->GetTimerManager().ClearTimer(TimerHandle_End);


		W->GetTimerManager().SetTimer(
			TimerHandle_Interp,
			[this, Avatar]()
			{
				const float RealDelta = FApp::GetDeltaTime();
				CurrentTimeDilation = FMath::FInterpTo(CurrentTimeDilation, TargetTimeDilation, RealDelta, InterpSpeed);
				Avatar->CustomTimeDilation = CurrentTimeDilation;
				if (MID_TheWorld)
				{
					float CurrentStrength;
					MID_TheWorld->GetScalarParameterValue(FMaterialParameterInfo(ScalarParameterName), CurrentStrength);
					float NewStrength = FMath::FInterpTo(CurrentStrength, 1.0f, RealDelta, InterpSpeed);
					MID_TheWorld->SetScalarParameterValue(ScalarParameterName, NewStrength);
				}

				if (FMath::Abs(CurrentTimeDilation - TargetTimeDilation) < 0.05f)
				{
					Avatar->CustomTimeDilation = TargetTimeDilation;
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interp);
					MID_TheWorld->SetScalarParameterValue(ScalarParameterName, 1.f);
				}
			},
			0.01f, true);


		W->GetTimerManager().SetTimer(
			TimerHandle_End,
			[this]()
			{
				QuitTheWorld();
			},
			1.25f, false);

	}
}

void UVoidTheWorld::QuitTheWorld()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interp);

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle_Interp,
		[this, Avatar]()
		{
			const float RealDelta = FApp::GetDeltaTime();
			CurrentTimeDilation = FMath::FInterpTo(CurrentTimeDilation, 1.0f, RealDelta, InterpSpeed * 2.f);
			Avatar->CustomTimeDilation = CurrentTimeDilation;

			if (MID_TheWorld)
			{
				float CurrentStrength;
				MID_TheWorld->GetScalarParameterValue(FMaterialParameterInfo(ScalarParameterName), CurrentStrength);
				float NewStrength = FMath::FInterpTo(CurrentStrength, 0.0f, RealDelta, InterpSpeed);
				MID_TheWorld->SetScalarParameterValue(ScalarParameterName, NewStrength);
			}

			if (FMath::Abs(CurrentTimeDilation - 1.0f) < 0.05f)
			{
				Avatar->CustomTimeDilation = 1.0f;
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Interp);
				UGameplayStatics::SetGlobalTimeDilation(Avatar->GetWorld(), 1.0f);
				MID_TheWorld->SetScalarParameterValue(ScalarParameterName, 0.f);

				FGameplayAbilityActorInfo ActorInfo = GetActorInfo();
				EndAbility(GetCurrentAbilitySpecHandle(), &ActorInfo, CurrentActivationInfo, true, false);
			}
		},
		0.01f, true);

}



void UVoidTheWorld::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearAllTimersForObject(this);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVoidTheWorld::BeginDestroy()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearAllTimersForObject(this);
	}
	Super::BeginDestroy();
}



