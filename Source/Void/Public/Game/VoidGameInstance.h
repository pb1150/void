// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "VoidGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class VOID_API UVoidGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UVoidGameInstance();

	/* Player Data */
	bool IsInit = true;

	int32 PlayerLevel = 1.f;
	int32 XP;

	float Health;
	float AddedMaxHealth;
	float MoveSpeedMultiplier;
	float SprintSpeedMultiplier;
	float HealthRegenRate;
	float CriticalHitChance;
	float CriticalHitMultiplier;
	float DamageReduction;
	float BlockChance;
	float AttackSpeedMultiplier;
	float MaxJumpCount;

	FGameplayTag WeaponTag;

	TArray<FAttributeEffectActors> PickedupActors;
	FGameplayTagContainer AbilitiesTag;

	/* End Player Data */

	
	ELevelType CurrentLevelType = ELevelType::Moon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelInfo")
	TObjectPtr<ULevelInfo> LevelDataAsset;

	int32 CurrentLevelIndex = 0;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowLoadingScreen();

	UFUNCTION(BlueprintImplementableEvent)
	void HideLoadingScreen();

	void OnVoidLevelLoaded();

	int32 GetThisLevelMaxKilledEnemyCount();

	/** 游戏运行开始时的时间（秒） */
	UPROPERTY(VisibleAnywhere)
	float GameStartTime = 0.f;

	/** 获取当前总运行时间(分） */
	UFUNCTION(BlueprintCallable, Category = "Game Runtime")
	int32 GetTotalRuntimeToInt() const;

protected: 
	virtual void Init() override;

	
};
