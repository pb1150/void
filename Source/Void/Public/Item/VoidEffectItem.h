// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/VoidItem.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "VoidEffectItem.generated.h"


class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectZoneType : uint8
{
	None,
	Fire,
	Poison,
	Ice,
	Heal
};

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyEndOverLap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};


/**
 * 
 */
UCLASS()
class VOID_API AVoidEffectItem : public AVoidItem
{
	GENERATED_BODY()
	
public:
	AVoidEffectItem();

	// 用 EffectZoneType 分类全局表
	static TMap<EEffectZoneType, TMap<TWeakObjectPtr<AActor>, int32>> GlobalOverlapCounts;
	static TMap<EEffectZoneType, TMap<FActiveGameplayEffectHandle, TWeakObjectPtr<UAbilitySystemComponent>>> GlobalActiveEffectHandles;


	/* Use for special attack like Redbuff */
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void EndOverlap(AActor* TargetActor);

	/** Effect class And Appply Policy */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	bool bApplyEffectsToEnemy = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	EEffectZoneType EffectZoneType = EEffectZoneType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Effect")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Effect")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Effect")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Effect")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes Effect")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	/** End Effect class And Appply Policy */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float ActorLevel = 1.f;
	
private:

	UPROPERTY(EditAnywhere, Category = "Effect|Sound")
	TObjectPtr<USoundBase> PickupSound;
	
	UPROPERTY(EditAnywhere, Category = "Effect|Sound")
	bool bPlaySound = false;
};
