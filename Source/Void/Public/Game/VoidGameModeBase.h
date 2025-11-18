// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VoidGameModeBase.generated.h"

class UCharacterClassInfo;
class UWeaponInfo;
class UVoidInputConfig;
class UInputKeyDisplayData;
class UAbilityInfo;
class AEnemySpawnPoint;
class AVoidEnemy;
class ICombatInterface;
class UVoidGameInstance;

/**
 * 
 */
UCLASS()
class VOID_API AVoidGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* LocalPlayer) override;

	/* Enemy Information */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWeaponInfo> WeaponClassInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UVoidInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputKeyDisplayData> KeyDisplayData;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drop")
	int32 CurrentAttributeEffectItemNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drop")
	int32 CurrentAbilityEffectItemNum = 0;



protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Transmitter|Spawn")
	TSubclassOf<AActor> TransmitterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Spawn")
	int32 MaxValidEnemyCount = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Spawn")
	float SpawnInterval = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Spawn")
	float InitialSpawnDelay = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Spawn")
	float ValidSpawnRadius = 3000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Spawn")
	int32 CurrentEnemyCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Spawn")
	int32 KilledEnemyCount = 0;

private:

	int32 MaxKilledEnemyCount;
	TArray<AActor*> Players;
	TObjectPtr<AActor> Player;
	TArray<AActor*> Transmitters;
	TObjectPtr<AActor> Transmitter;
	FTimerHandle EnemySpawnTimerHandle;
	TArray<AEnemySpawnPoint*> AllSpawnPoints;
	TArray<AEnemySpawnPoint*> AvailableSpawnPoints;
	ICombatInterface* PlayerInterface;
	TObjectPtr<UVoidGameInstance> VoidGameInstance = nullptr;
		
	UFUNCTION()
	void OnEnemyKilled(AActor* EnemyActor);

	void TrySpawnEnemies();
	AEnemySpawnPoint* GetRandomAvailableSpawnPoint();
	TSubclassOf<AVoidEnemy> GetRandomEnemyClassFromData();
};
