// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VoidCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "VoidEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AVoidAIController;
class UAnimMontage;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnFinishSignature, bool, IsSpawning);

/**
 * 
 */
UCLASS()
class VOID_API AVoidEnemy : public AVoidCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AVoidEnemy();
	virtual void PossessedBy(AController* NewController) override;

	/** Enemy Interface */
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	virtual UAnimMontage* GetMeleeAttackMontage_Implementation() override;
	virtual int32 GetMeleeMontageSectionCount_Implementation() override;
	virtual void SetCombatTarget_Implementation(AActor* InTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	virtual FVector GetRightCombatSocketLocation_Implementation() override;
	virtual FVector GetLeftCombatSocketLocation_Implementation() override;
	virtual FVector GetSpawnActorLocation_Implementation() override;
	virtual UAnimMontage* GetSpawnActorMontage_Implementation() override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual UParticleSystem* GetSpawnParticleSystem_Implementation() override;
	virtual FVector GetSpawnParticleSystemLocation_Implementation() override;
	virtual UAnimMontage* GetSpawnAnimMontage_Implementation() override;
	virtual void EndSpawnEnemy_Implementation() override;
	/** End Enemy Interface */ 

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	virtual FVector GetDamageTextSocketLocation() override;

	/** End Combat Interface */

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChangedDelegate;

	UPROPERTY(BlueprintReadOnly)
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseWalkSpeed = 400.f;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy|Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Character Default", meta = (ExposeOnSpawn = true))
	int32 Level = 1;

	UPROPERTY(BlueprintAssignable)
	FOnSpawnFinishSignature OnSpawnFinishDelegate;
	bool IsSpawning = true;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() override;
	void HitReactTagChanged(const FGameplayTag Tag, int32 Count);

	FVector SpawnParticleSystemLocation = FVector();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AVoidAIController> VoidAIController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Character Default")
	ECharacterClass CharacterClass = ECharacterClass::RedBuff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealthBarScale = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Socket")
	FName RightCombatSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Socket")
	FName LeftCombatSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Socket")
	FName SpawnActorSocket;

	/** Montage */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montage")
	TObjectPtr<UAnimMontage> MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montage")
	int32 MeleeMontageSectionCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|Montage")
	TObjectPtr<UAnimMontage> SpawnActorMontage;

	/** End Montage */


private:
	void OnHealthChange(const FOnAttributeChangeData& Data);
	void OnMaxHealthChange(const FOnAttributeChangeData& Data);


	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Material")
	TObjectPtr<UMaterial> M_HighLight;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Spawn")
	TObjectPtr<UParticleSystem> SpawnParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Spawn")
	TObjectPtr<UAnimMontage> SpawnMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Sound")
	TObjectPtr<USoundBase> EnemyDeadSound;
};
