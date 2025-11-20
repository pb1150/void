// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "AbilitySystem/Data/WeaponInfo.h"
#include "GameplayTagContainer.h"
#include "Game/VoidSaveGame.h"
#include "VoidBlueprintFunctionLibrary.generated.h"

class UDetailsWidgetController;
class UChooseMenuWidgetController;
class AVoidWeapon;
class USkeletalMeshComponent;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class VOID_API UVoidBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    /** Get the absolute position of the control in the viewport (top left corner) */
    UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Void|UI")
    static FVector2D GetWidgetViewportPosition(UObject* WorldContextObject, UWidget* Widget, bool bReturnCenter = false);

    UFUNCTION(BlueprintPure, Category = "Void|WidgetController")
    static UDetailsWidgetController* GetDetailsWidgetController(UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Void|WidgetController")
    static UChooseMenuWidgetController* GetChooseMenuWidgetController(UObject* WorldContextObject);
	
    UFUNCTION(BlueprintCallable, Category = "Void|CharacterClassDefault")
    static void InitializeDefaultAttributes(UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

    UFUNCTION(BlueprintCallable, Category = "Void|CharacterClassDefault")
    static void GiveStartupAbilities(UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level);

    UFUNCTION(BlueprintCallable, Category = "Void|CharacterClassDefault")
    static UCharacterClassInfo* GetCharacterClassInfo(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Void|CharacterClassDefault")
    static UTexture2D* GetCharacterClassIconByCharacterClass(UObject* WorldContextObject, ECharacterClass CharacterClass);

    UFUNCTION(BlueprintCallable, Category = "Void|CharacterClassDefault")
    static UTexture2D* GetCharacterClassIconByCharacterClassFromGI(UObject* WorldContextObject, ECharacterClass CharacterClass);

    UFUNCTION(BlueprintCallable, Category = "Void")
    static void GetAbilitiesInfoFromGI(UObject* WorldContextObject, TArray<FVoidAbilityInfo>& AbilitiesInfo);

    UFUNCTION(BlueprintCallable, Category = "Void")
    static void GetAbilitiesInfoByInputTagFromGI(UObject* WorldContextObject, TArray<FVoidAbilityInfo>& AbilitiesInfo, FGameplayTag InputTag);
    
    UFUNCTION(BlueprintCallable, Category = "Void")
    static const FLevelDefaultInfo GetCurrentLevelInfo(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Void")
    static int32 GetLastReachedLevel(UObject* WorldContextObject);

    UFUNCTION(BlueprintPure, Category = "Void|GameplayEffects")
    static bool IsCanHitReact(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "Void|GameplayEffects")
    static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "Void|GameplayEffects")
    static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintCallable, Category = "Void|GameplayEffects")
    static void SetCanHitReact(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InCanHitReact);

    UFUNCTION(BlueprintCallable, Category = "Void|GameplayEffects")
    static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInBlockedHit);

    UFUNCTION(BlueprintCallable, Category = "Void|GameplayEffects")
    static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInCriticalHit);

    UFUNCTION(BlueprintCallable, Category = "Void|WeaponClassDefault")
    static UWeaponInfo* GetWeaponClassInfo(UObject* WorldContextObject);

    static UAbilityInfo* GetAbilityInfo(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Void|WeaponDefault")
    static FGameplayTag GetGameplayCueByWeaponTag(const FGameplayTag& WeaponTag);

    static void SpawnAndEquipWeapon(UObject* WorldContextObject, AVoidWeapon*& HeldWeapon, const FGameplayTag& WeaponTag, USkeletalMeshComponent* Mesh);

    UFUNCTION(BlueprintCallable, Category = "Void|GameplayMechanics")
    static void GetPlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

    UFUNCTION(BlueprintCallable, Category = "Void|GameplayMechanics")
    static void GetEnemysWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

    UFUNCTION(BlueprintCallable, Category = "Void|Input")
    static FName GetKeyboardByInputActionTag(const UObject* WorldContextObject, const FGameplayTag& InputActionTag);

    /* ZImpluse is the Strength of Z*/
    UFUNCTION(BlueprintCallable, Category = "Void|Movement")
    static void AddHitImpluse(AActor* TargetActor, AActor* SourceActor, float ImpulseStrength, float ZImpluse = 0.5f);

    static void AddAttractionForce(AActor* TargetActor, AActor* SourceActor, float AttractionStrength, float ZForce);

    static void AddAttractionForce(AActor* TargetActor, AActor* SourceActor, float AttractionStrength);

    static void AddAttractionForceWithRadius(AActor* TargetActor, AActor* SourceActor, float AttractionStrength, float Radius);

    static int32 GetXPRewardForCharacterClassAndLevel(UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

    /* Save player data during game */
    static void TempSaveCharacterProgress(ACharacter* Character, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

    static void LoadCharacterAttribute(ACharacter* Character, UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayEffect> DefaultLevel_SetByCaller, TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes, TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes_SetByCaller);

    static void LoadCharacterAbilties(ACharacter* Character, TArray<TSubclassOf<UGameplayAbility>>& OwnedAbilities);

    UFUNCTION(BlueprintCallable, Category = "Void|SpawnActor")
    static void SpawnDropItems(UObject* WorldContextObject, ECharacterClass CharacterClass, FVector SpawnLocation);

    UFUNCTION(BlueprintCallable, Category = "Void|Level")
    static void LoadNextLevel(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Void|Level")
    static void LoadLevelByIndex(UObject* WorldContextObject, int32 Index);

    static void BroadcastAbilityInfoOnPLayerDead(UAbilitySystemComponent* ASC);

    UFUNCTION(BlueprintPure, Category = "Void")
    static float GetAttackSpeedMultiplier(UAbilitySystemComponent* AbilitySystemComponent);

    /* Called */
    UFUNCTION(BlueprintCallable, Category = "Void|Sound")
    static void PlayGlobalMusic(UObject* WorldContextObject, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f);

    UFUNCTION(BlueprintCallable, Category = "Void|Sound")
    static void StopGlobalMusic(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable)
    static void RecordStartTime(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Void|Save")
    static void SavePlayHistory(UObject* WorldContextObject, const UAbilitySystemComponent* ASC, ECharacterClass KilledByCharacterClass, ACharacter* Player);

    UFUNCTION(BlueprintCallable, Category = "Void|Save")
    static int32 LoadAllPlayHistory(UObject* WorldContextObject, TArray<FPlayHistory>& OutHistoryList);

    UFUNCTION(BlueprintCallable, Category = "Void|Save")
    static FText DateTimeToText(const FDateTime& DateTime);

    UFUNCTION(BlueprintPure, Category = "Void|Util")
    static FText FormatSecondsToText(float Seconds);

    UFUNCTION(BlueprintCallable, Category = "Void|Save")
    static void SavePlayerStartupAbilities(UObject* WorldContextObject, const FGameplayTag AbilityTag, const FGameplayTag InputTag);
};
