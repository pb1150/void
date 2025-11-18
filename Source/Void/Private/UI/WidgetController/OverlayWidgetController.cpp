// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/VoidAttributeSet.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Player/VoidPlayerController.h"
#include "Player/VoidPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/VoidUserWidget.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	UVoidAttributeSet* VoidAttributeSet = CastChecked<UVoidAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(VoidAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(VoidAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	UVoidAttributeSet* VoidAttributeSet = CastChecked<UVoidAttributeSet>(AttributeSet);
	AVoidPlayerState* VoidPlayerState = CastChecked<AVoidPlayerState>(PlayerState);
	AVoidPlayerController* VoidPC = CastChecked<AVoidPlayerController>(PlayerController);

	VoidPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	VoidPlayerState->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnLevelChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VoidAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(VoidAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	Cast<UVoidAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddUObject(this, &UOverlayWidgetController::CallbackMessage);
	Cast<UVoidAbilitySystemComponent>(AbilitySystemComponent)->AbilityChangedDelegate.AddUObject(this, &UOverlayWidgetController::CallbackAbilityChanged);

	VoidPC->OnOpenDetails.AddUObject(this, &UOverlayWidgetController::ShowDetailsMenu);
	VoidPC->OnCloseDetails.AddUObject(this, &UOverlayWidgetController::CloseDetailsMenu);
	VoidPC->OnSprintChangedDelegate.AddUObject(this, &UOverlayWidgetController::ChangedCrosshair);
	VoidPC->OnPauseGame.AddUObject(this, &UOverlayWidgetController::ShowPauseGameMenu);
	VoidPC->OnUnPauseGame.AddUObject(this, &UOverlayWidgetController::ClosePauseGameMenu);

	VoidAttributeSet->OnPlayerDead.BindUObject(this, &UOverlayWidgetController::PlayerDead);
}


void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::CallbackMessage(const FGameplayTagContainer& TagContainer)
{
	for (const FGameplayTag& Tag : TagContainer)
	{
		if (!Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Message"))) return;

		const FUIWidgetRow* WidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
		if (WidgetRow != nullptr) MessageWidgetRowDelegate.Broadcast(*WidgetRow);
	}

}

void UOverlayWidgetController::CallbackAbilityChanged(const FAbilityMessage& AbilityMessage)
{
	FVoidAbilityInfo AbilityInfo = UVoidBlueprintFunctionLibrary::GetAbilityInfo(this)->FindAbilityInfoByTag(AbilityMessage.AbilityTag);
	AbilityInfo.InputTag = AbilityMessage.InputActionTag;
	AbilityChanged.Broadcast(AbilityInfo);
}

void UOverlayWidgetController::ShowDetailsMenu()
{
	OnOpenDetailsDelegate.Broadcast();
}

void UOverlayWidgetController::CloseDetailsMenu()
{
	OnCloseDetailsDelegate.Broadcast();
}

void UOverlayWidgetController::ChangedCrosshair(bool NewValue)
{
	OnPlayerSprintChangedDelegate.Broadcast(NewValue);
}

void UOverlayWidgetController::ShowPauseGameMenu()
{
	OnShowPauseMenuDelegate.Broadcast();
}

void UOverlayWidgetController::ClosePauseGameMenu()
{
	OnClosePauseMenuDelegate.Broadcast();
}

void UOverlayWidgetController::ClosePauseMenuByButton()
{
	if (PlayerController && Cast<AVoidPlayerController>(PlayerController))
	{
		Cast<AVoidPlayerController>(PlayerController)->PauseGameByButton();
	}
}
		

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	AVoidPlayerState* VoidPlayerState = CastChecked<AVoidPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = VoidPlayerState->LevelUpInfo;

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num() - 1;
	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousRequirement;
		const int32 XPForThisLevel = NewXP - PreviousRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnXPPrecentChangedDelegate.Broadcast(XPBarPercent);
	}
}


void UOverlayWidgetController::OnLevelChanged(int32 NewLevel)
{
	OnLevelChangedDelegate.Broadcast(NewLevel);
}

void UOverlayWidgetController::PlayerDead(ECharacterClass CharacterClass)
{
	FPlayerDeadData DeadData;
	DeadData.EnemyClass = CharacterClass;
	AVoidPlayerState* VoidPS = CastChecked<AVoidPlayerState>(PlayerState);
	if (VoidPS)
	{
		DeadData.PlayerLevel = VoidPS->GetPlayerLevel();
	}
	
	OnPlayerDeadDelegate.Broadcast(DeadData);
}

