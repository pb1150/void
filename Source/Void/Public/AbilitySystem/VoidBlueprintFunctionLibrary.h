// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/Data/WeaponInfo.h"
#include "GameplayTagContainer.h"
#include "VoidBlueprintFunctionLibrary.generated.h"

class UDetailsWidgetController;
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
    UFUNCTION(BlueprintPure, Category = "Widget", meta = (WorldContext = "WorldContextObject"))
    static FVector2D GetWidgetViewportPosition(UObject* WorldContextObject, UWidget* Widget, bool bReturnCenter = false);

    UFUNCTION(BlueprintPure, Category = "ASC|WidgetController")
    static UDetailsWidgetController* GetDetailsWidgetController(UObject* WorldContextObject);
	
    UFUNCTION(BlueprintCallable, Category = "ASC|CharacterClassDefault")
    static void InitializeDefaultAttributes(UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

    UFUNCTION(BlueprintCallable, Category = "ASC|CharacterClassDefault")
    static void GiveStartupAbilities(UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level);

    UFUNCTION(BlueprintCallable, Category = "ASC|CharacterClassDefault")
    static UCharacterClassInfo* GetCharacterClassInfo(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "ASC|CharacterClassDefault")
    static UTexture2D* GetCharacterClassIconByCharacterClass(UObject* WorldContextObject, ECharacterClass CharacterClass);

    UFUNCTION(BlueprintPure, Category = "ASC|GameplayEffects")
    static bool IsCanHitReact(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "ASC|GameplayEffects")
    static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintPure, Category = "ASC|GameplayEffects")
    static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

    UFUNCTION(BlueprintCallable, Category = "ASC|GameplayEffects")
    static void SetCanHitReact(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InCanHitReact);

    UFUNCTION(BlueprintCallable, Category = "ASC|GameplayEffects")
    static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInBlockedHit);

    UFUNCTION(BlueprintCallable, Category = "ASC|GameplayEffects")
    static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInCriticalHit);

    UFUNCTION(BlueprintCallable, Category = "ASC|WeaponClassDefault")
    static UWeaponInfo* GetWeaponClassInfo(UObject* WorldContextObject);

    static UAbilityInfo* GetAbilityInfo(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "ASC|WeaponDefault")
    static FGameplayTag GetGameplayCueByWeaponTag(const FGameplayTag& WeaponTag);

    static void SpawnAndEquipWeapon(UObject* WorldContextObject, AVoidWeapon*& HeldWeapon, const FGameplayTag& WeaponTag, USkeletalMeshComponent* Mesh);

    UFUNCTION(BlueprintCallable, Category = "ASC|GameplayMechanics")
    static void GetPlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

    UFUNCTION(BlueprintCallable, Category = "ASC|GameplayMechanics")
    static void GetEnemysWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

    UFUNCTION(BlueprintCallable, Category = "ASC|Input")
    static FName GetKeyboardByInputActionTag(const UObject* WorldContextObject, const FGameplayTag& InputActionTag);

    /* ZImpluse is the Strength of Z*/
    UFUNCTION(BlueprintCallable, Category = "ASC|Movement")
    static void AddHitImpluse(AActor* TargetActor, AActor* SourceActor, float ImpulseStrength, float ZImpluse = 0.5f);

    static void AddAttractionForce(AActor* TargetActor, AActor* SourceActor, float AttractionStrength, float ZForce);

    static void AddAttractionForce(AActor* TargetActor, AActor* SourceActor, float AttractionStrength);

    static void AddAttractionForceWithRadius(AActor* TargetActor, AActor* SourceActor, float AttractionStrength, float Radius);

    static int32 GetXPRewardForCharacterClassAndLevel(UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

    /* Save player data during game */
    static void TempSaveCharacterProgress(ACharacter* Character, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

    static void LoadCharacterAttribute(ACharacter* Character, UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayEffect> DefaultLevel_SetByCaller, TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes, TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes_SetByCaller);

    static void LoadCharacterAbilties(ACharacter* Character, TArray<TSubclassOf<UGameplayAbility>>& OwnedAbilities);

    UFUNCTION(BlueprintCallable, Category = "SpawnActor")
    static void SpawnDropItems(UObject* WorldContextObject, ECharacterClass CharacterClass, FVector SpawnLocation);

    UFUNCTION(BlueprintCallable, Category = "Level")
    static void LoadNextLevel(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Level")
    static void LoadLevelByIndex(UObject* WorldContextObject, int32 Index);

    static void BroadcastAbilityInfoOnPLayerDead(UAbilitySystemComponent* ASC);

    UFUNCTION(BlueprintPure, Category = "ASC")
    static float GetAttackSpeedMultiplier(UAbilitySystemComponent* AbilitySystemComponent);
};
