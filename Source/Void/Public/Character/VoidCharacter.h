// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VoidCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "Item/Weapon/VoidWeapon.h"
#include "GameplayTagContainer.h"
#include "VoidCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class AVoidWeapon;
class AVoidMeleeWeapon;
class UCableComponent;
struct FOnAttributeChangeData;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None		UMETA(DisplayName = "None"),
	Gun         UMETA(DisplayName = "Gun"),
	Sword       UMETA(DisplayName = "Sword"),
	BaseballBat UMETA(DisplayName = "Baseball Bat")
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponTypeChanged, EWeaponType, WeaponType);


/**
 * 
 */
UCLASS()
class VOID_API AVoidCharacter : public AVoidCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()


public:

	AVoidCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(BlueprintAssignable)
	FOnWeaponTypeChanged OnWeaponTypeChangedDelegate;


	/** Combat Interface */
	virtual FVector GetRifleBulletSocketLocation_1_Implementation() override;
	virtual FRotator GetRifleBulletSocketRotation_1_Implementation() override;
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/** End Combat Interface */

	/** Player Interface */
	virtual bool PlayerSprint() override;
	virtual void SwitchToWalk_Implementation() override;
	virtual AVoidWeapon* GetActorHeldWeapon_Implementation() override;
	virtual FGameplayTag GetHeldWeaponTag_Implementation() override;
	virtual AVoidMeleeWeapon* GetActorHeldMeleeWeapon_Implementation() override;
	virtual void EquipWeaponByTag(const FGameplayTag& Tag) override;
	virtual void AddToXp_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXp_Implementation() const override;
	virtual int32 FindLevelForXp_Implementation(int32 InXP) const override;
	virtual void AddToPlayerLevel_Implementation(int32 PlayerLevel) override;
	virtual void TempSaveCharacterProgress_Implementation() override;
	virtual USceneComponent* GetCableTargetComponent_Implementation() override;
	virtual UCableComponent* GetCableComponent_Implementation() override;
	/** End Player Interface */

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void InitAbilityActorInfo() override;

	void MoveSpeedMultiplierChanged(const FOnAttributeChangeData& Data);
	void SprintSpeedMultiplierChanged(const FOnAttributeChangeData& Data);

	EWeaponType FindWeaponTypeByTag(const FGameplayTag& WeaponTag);
	void EquipWeapon(const FGameplayTag& Tag);
	void LoadProgress();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float BaseWalkSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float BaseSprintSpeed = 600.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentWalkSpeed = 400.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentSprintSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkFOV = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintFOV = 110.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float FOVInterpSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> LevelUpSound;

	/** Weapon */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag HeldWeaponTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AVoidWeapon* HeldWeapon;
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCableComponent> CableComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> CableTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	TMap<FGameplayTag, EWeaponType> WeaponInfoMap;


};
