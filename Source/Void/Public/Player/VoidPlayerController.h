// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/ItemInterface.h"
#include "GameplayTagContainer.h"
#include "VoidPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UVoidInputConfig;
class UVoidAbilitySystemComponent;
class UDamageTextComponent;
struct FInputActionValue;

DECLARE_MULTICAST_DELEGATE(FOnPCChangeDetailsSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMovementStateChangedSignature, bool);

/**
 * 
 */
UCLASS()
class VOID_API AVoidPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AVoidPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);
	FORCEINLINE UInputMappingContext* GetVoidContext() const { return VoidContext; }
	
	FOnPCChangeDetailsSignature OnOpenDetails;
	FOnPCChangeDetailsSignature OnCloseDetails;
	FOnMovementStateChangedSignature OnSprintChangedDelegate;
	FOnPCChangeDetailsSignature OnPauseGame;
	FOnPCChangeDetailsSignature OnUnPauseGame;

	void PauseGameByButton();

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;


private:

	void EnemyBoxTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	
	void ItemBoxTrace(float DeltaTime);
	IItemInterface* LastItem;
	IItemInterface* ThisItem;


	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> VoidContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShowDetailsAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SwitchWeaponAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> PauseGameAction;


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	bool bSwitchWeaponTiming = false;
	float SwitchWeaponTriggerTime = 0.f;


	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StopJumping(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void OpenDetails(const FInputActionValue& Value);
	void CloseDetails(const FInputActionValue& Value);
	void TriggerSwitchWeapon(const FInputActionValue& Value);
	void CompletedSwitchWeapon(const FInputActionValue& Value);
	void PauseGame(const FInputActionValue& Value);


	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UVoidInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UVoidAbilitySystemComponent> VoidASC;

	UVoidAbilitySystemComponent* GetVoidASC();
};
