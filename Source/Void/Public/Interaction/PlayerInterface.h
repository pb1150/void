// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "PlayerInterface.generated.h"

class AVoidWeapon;
class AVoidMeleeWeapon;
class UCameraComponent;
class UCableComponent;
struct FInputActionValue;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VOID_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/* return bIsSprinting */
	virtual bool PlayerSprint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwitchToWalk();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FGameplayTag GetHeldWeaponTag();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AVoidWeapon* GetActorHeldWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AVoidMeleeWeapon* GetActorHeldMeleeWeapon();

	virtual void EquipWeaponByTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintNativeEvent)
	void AddToXp(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXp() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXp(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 PlayerLevel);

	virtual UCameraComponent* GetMenuCamera() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TempSaveCharacterProgress();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USceneComponent* GetCableTargetComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCableComponent* GetCableComponent();

};
