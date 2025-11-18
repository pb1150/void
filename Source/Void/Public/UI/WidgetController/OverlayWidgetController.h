// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/VoidWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AttributeEffectActorsInfo.h"
#include "OverlayWidgetController.generated.h"

class UVoidUserWidget;
struct FOnAttributeChangeData;
struct FAbilityMessage;
enum class ECharacterClass : uint8;


USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemInfo = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EActorQuality ActorQuality = EActorQuality::Common;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UVoidUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

USTRUCT(BlueprintType)
struct FPlayerDeadData
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	ECharacterClass EnemyClass;

	UPROPERTY(BlueprintReadWrite, Category = "Player")
	int32 PlayerLevel = 1;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityChangedSignature, FVoidAbilityInfo, AbilityInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeDetailsSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementChangedSignature, bool, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDieSignature, const FPlayerDeadData&, Data);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VOID_API UOverlayWidgetController : public UVoidWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override; 

	UFUNCTION(BlueprintCallable)
	void ClosePauseMenuByButton();

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnPlayerStateChangedSignature OnLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnAttributeChangedSignature OnXPPrecentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FAbilityChangedSignature AbilityChanged;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnChangeDetailsSignature OnOpenDetailsDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnChangeDetailsSignature OnCloseDetailsDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnMovementChangedSignature OnPlayerSprintChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnChangeDetailsSignature OnShowPauseMenuDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnChangeDetailsSignature OnClosePauseMenuDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FOnPlayerDieSignature OnPlayerDeadDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void CallbackMessage(const FGameplayTagContainer& TagContainer);
	void CallbackAbilityChanged(const FAbilityMessage& AbilityMessage);

	template<class T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void ShowDetailsMenu();
	void CloseDetailsMenu();
	void ChangedCrosshair(bool NewValue);
	void ShowPauseGameMenu();
	void ClosePauseGameMenu();

private:

	void OnXPChanged(int32 NewXP);
	void OnLevelChanged(int32 NewLevel);
	void PlayerDead(ECharacterClass CharacterClass);

};

template<class T>
inline T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), FString());
}
