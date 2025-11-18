// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "VoidAttributeSet.generated.h"

enum class ECharacterClass : uint8;

DECLARE_DELEGATE_OneParam(FOnPlayerDeadSignature, ECharacterClass);

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

};




/**
 * 
 */
UCLASS()
class VOID_API UVoidAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UVoidAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, FGameplayAttribute> TagstoAttributes;

	FOnPlayerDeadSignature OnPlayerDead;

	/*
	* Level 
	*/


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Level, Category = "Primary Attributes")
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, Level);


	/*
	* Primary Attributes
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Primary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, Category = "Primary Attributes")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, Damage);


	/*
	* Secondary Attributes
	*/

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Secondary Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AddedMaxHealth, Category = "Secondary Attributes")
	FGameplayAttributeData AddedMaxHealth;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, AddedMaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeedMultiplier, Category = "Secondary Attributes")
	FGameplayAttributeData MoveSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, MoveSpeedMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SprintSpeedMultiplier, Category = "Secondary Attributes")
	FGameplayAttributeData SprintSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, SprintSpeedMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegenRate, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, HealthRegenRate);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitMultiplier, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitMultiplier;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, CriticalHitMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageReduction, Category = "Secondary Attributes")
	FGameplayAttributeData DamageReduction;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, DamageReduction);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeedMultiplier, Category = "Secondary Attributes")
	FGameplayAttributeData AttackSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, AttackSpeedMultiplier);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxJumpCount, Category = "Secondary Attributes")
	FGameplayAttributeData MaxJumpCount;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, MaxJumpCount);

	/*
	* Meta Attributes
	*/

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UVoidAttributeSet, IncomingXP);


	
	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldLevel) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldDamage) const;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_AddedMaxHealth(const FGameplayAttributeData& OldAddedMaxHealth) const;

	UFUNCTION()
	void OnRep_MoveSpeedMultiplier(const FGameplayAttributeData& OldMoveSpeedMultiplier) const;

	UFUNCTION()
	void OnRep_SprintSpeedMultiplier(const FGameplayAttributeData& OldSprintSpeedMultiplier) const;

	UFUNCTION()
	void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitMultiplier(const FGameplayAttributeData& OldCriticalHitMultiplier) const;

	UFUNCTION()
	void OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_AttackSpeedMultiplier(const FGameplayAttributeData& OldAttackSpeedMultiplier) const;

	UFUNCTION()
	void OnRep_MaxJumpCount(const FGameplayAttributeData& OldMaxJumpCount) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	void ShowFloatingText(const FEffectProperties& Props, float LocalIncomingDamage, bool bBlockedHit, bool bCriticalHit) const;
	void SendXPEvent(const FEffectProperties& Props);

};
