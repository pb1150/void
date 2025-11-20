// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/ChooseMenuWidgetController.h"

UChooseMenuWidgetController::UChooseMenuWidgetController()
{
}

void UChooseMenuWidgetController::BroadcastAbilityChange(FGameplayTag AbilityTag, FGameplayTag InputTag)
{
	OnAbilityChangeDelegate.Broadcast(AbilityTag, InputTag);
}

void UChooseMenuWidgetController::BroadcastOnLevelChange()
{
	OnChangeLevelDelegate.Broadcast();
}
