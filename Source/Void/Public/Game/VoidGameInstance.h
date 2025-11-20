// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "VoidGameInstance.generated.h"

class UAudioComponent;
class UCharacterClassInfo;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class VOID_API UVoidGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UVoidGameInstance();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilityInfo> AbilitiesInfo;

	/* Player Data */
	bool IsInit = true;

	int32 PlayerLevel = 1;
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

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelInfo")
	TObjectPtr<ULevelInfo> LevelDataAsset;

	int32 CurrentLevelIndex = 0;
	int32 LastReachedLevel = 0;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowLoadingScreen();

	UFUNCTION(BlueprintImplementableEvent)
	void HideLoadingScreen();

	void OnVoidLevelLoaded();

	int32 GetThisLevelMaxKilledEnemyCount();

	/** 这一局游戏开始时的真实日期时间（用于存档显示） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Runtime")
	FDateTime StartTime;

	/** 游戏运行开始时的时间（秒） */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Runtime")
	float GameStartTime = 0.f;

	/** 获取当前总运行时间(分） */
	UFUNCTION(BlueprintCallable, Category = "Game Runtime")
	int32 GetTotalRuntimeToInt() const;

	UPROPERTY()
	TObjectPtr<UAudioComponent> GlobalMusicComp = nullptr;

protected: 
	virtual void Init() override;


	
};
