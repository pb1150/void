// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/VoidWidgetController.h"
#include "GameplayTagContainer.h"
#include "ChooseMenuWidgetController.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityChangeSignature, FGameplayTag, AbilityTag, FGameplayTag, InputTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeLevelSignature);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VOID_API UChooseMenuWidgetController : public UVoidWidgetController
{
	GENERATED_BODY()

public:
	UChooseMenuWidgetController();

	UPROPERTY(BlueprintAssignable)
	FOnAbilityChangeSignature OnAbilityChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnChangeLevelSignature OnChangeLevelDelegate;

	UFUNCTION(BlueprintCallable)
	void BroadcastAbilityChange(FGameplayTag AbilityTag, FGameplayTag InputTag);

	UFUNCTION(BlueprintCallable)
	void BroadcastOnLevelChange();

};
