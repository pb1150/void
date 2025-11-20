// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Widget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/VoidHUD.h"
#include "Player/VoidPlayerState.h"
#include "Player/VoidPlayerController.h"
#include "Player/VoidMainMenuController.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UI/WidgetController/VoidWidgetController.h"
#include "Game/VoidGameModeBase.h"
#include "VoidGameplayTags.h"
#include "VoidAbilityTypes.h"
#include "Item/Weapon/VoidWeapon.h"
#include "Interaction/CombatInterface.h"
#include "Input/VoidInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "Input/InputKeyDisplayData.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/VoidGameInstance.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "AbilitySystem/VoidAttributeSet.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/DropItemsInfo.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Engine/AssetManager.h"
#include "AbilitySystem/Ability/VoidAbilityBase.h"



FVector2D UVoidBlueprintFunctionLibrary::GetWidgetViewportPosition(UObject* WorldContextObject, UWidget* Widget, bool bReturnCenter)
{
    if (!WorldContextObject || !Widget)
    {
        return FVector2D::ZeroVector;
    }

    const FGeometry Geometry = Widget->GetCachedGeometry();

    const FVector2D AbsolutePosition = Geometry.GetAbsolutePosition();

    FVector2D PixelPosition, ViewportPosition;
    USlateBlueprintLibrary::AbsoluteToViewport(WorldContextObject, AbsolutePosition, PixelPosition, ViewportPosition);

    if (bReturnCenter)
    {
        const FVector2D Size = Geometry.GetLocalSize();
        ViewportPosition += Size * 0.5f;
    }

    return ViewportPosition;
}

UDetailsWidgetController* UVoidBlueprintFunctionLibrary::GetDetailsWidgetController(UObject* WorldContextObject)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    if (PC)
    {
        AVoidHUD* VoidHUD = PC->GetHUD<AVoidHUD>();
        if(VoidHUD)
        {
            AVoidPlayerState* VoidPS = PC->GetPlayerState<AVoidPlayerState>();
            UAbilitySystemComponent* ASC = VoidPS->GetAbilitySystemComponent();
            UAttributeSet* AS = VoidPS->GetAttributeSet();
            FWidgetControllerParams WidgetControllerParams(PC, VoidPS, ASC, AS);
            return VoidHUD->GetDetailsWidgetController(WidgetControllerParams);

        }
    }
    return nullptr;
}

UChooseMenuWidgetController* UVoidBlueprintFunctionLibrary::GetChooseMenuWidgetController(UObject* WorldContextObject)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    if (PC == nullptr)  return nullptr;
    AVoidMainMenuController* MainMenuController = Cast<AVoidMainMenuController>(PC);
    if(MainMenuController == nullptr)  return nullptr;
    return MainMenuController->GetChooseMenuWidgetController();
}

void UVoidBlueprintFunctionLibrary::InitializeDefaultAttributes(UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
    UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
    FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetCharacterDefaultInfo(CharacterClass);

    FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
    ContextHandle.AddSourceObject(ASC->GetAvatarActor());
    FGameplayEffectSpecHandle LevelSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->LevelAttribute, 1.f, ContextHandle);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(LevelSpecHandle, FVoidGameplayTags::Get().Data_Enemy_Level, Level);
    ASC->ApplyGameplayEffectSpecToSelf(*LevelSpecHandle.Data.Get());


    FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(ClassInfo->PrimaryAttributes, 1.f, ContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

    FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.SecondaryAttributes, 1.f, ContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

}

void UVoidBlueprintFunctionLibrary::GiveStartupAbilities(UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level)
{
    UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
    if (ClassInfo == nullptr) return;

    for (TSubclassOf<UGameplayAbility> AbilityClass : ClassInfo->CommonAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
        ASC->GiveAbility(AbilitySpec);
    }

    const FCharacterClassDefaultInfo CharacterInfo = ClassInfo->GetCharacterDefaultInfo(CharacterClass);
    for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterInfo.Abilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
        ASC->GiveAbility(AbilitySpec);
    }

}

UCharacterClassInfo* UVoidBlueprintFunctionLibrary::GetCharacterClassInfo(UObject* WorldContextObject)
{
    AVoidGameModeBase* VoidGameMode = Cast<AVoidGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (VoidGameMode == nullptr) return nullptr;

    return VoidGameMode->CharacterClassInfo;
}

UTexture2D* UVoidBlueprintFunctionLibrary::GetCharacterClassIconByCharacterClass(UObject* WorldContextObject, ECharacterClass CharacterClass)
{
    AVoidGameModeBase* VoidGameMode = Cast<AVoidGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (VoidGameMode == nullptr) return nullptr;

    UCharacterClassInfo* ClassInfo = VoidGameMode->CharacterClassInfo;
    if (ClassInfo == nullptr) return nullptr;

    return ClassInfo->GetCharacterDefaultInfo(CharacterClass).EnemyIcon;
}

UTexture2D* UVoidBlueprintFunctionLibrary::GetCharacterClassIconByCharacterClassFromGI(UObject* WorldContextObject, ECharacterClass CharacterClass)
{
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr) return nullptr;

    UCharacterClassInfo* ClassInfo = VoidGameInstance->CharacterClassInfo;
    if (ClassInfo == nullptr) return nullptr;

    return ClassInfo->GetCharacterDefaultInfo(CharacterClass).EnemyIcon;
}

void UVoidBlueprintFunctionLibrary::GetAbilitiesInfoFromGI(UObject* WorldContextObject, TArray<FVoidAbilityInfo>& AbilitiesInfo)
{
    AbilitiesInfo.Empty();
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr || VoidGameInstance->AbilitiesInfo == nullptr || VoidGameInstance->AbilitiesInfo->AbilityInfomation.IsEmpty()) return;

    for (FVoidAbilityInfo Info : VoidGameInstance->AbilitiesInfo->AbilityInfomation)
    {
        AbilitiesInfo.AddUnique(Info);
    }
}

void UVoidBlueprintFunctionLibrary::GetAbilitiesInfoByInputTagFromGI(UObject* WorldContextObject, TArray<FVoidAbilityInfo>& AbilitiesInfo, FGameplayTag InputTag)
{
    AbilitiesInfo.Empty();
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr || VoidGameInstance->AbilitiesInfo == nullptr || VoidGameInstance->AbilitiesInfo->AbilityInfomation.IsEmpty()) return;

    for (FVoidAbilityInfo Info : VoidGameInstance->AbilitiesInfo->AbilityInfomation)
    {
        UVoidAbilityBase* Ability = NewObject<UVoidAbilityBase>(WorldContextObject, Info.AbilityClass);
        if (Ability)
        {
            if (Ability->StartupInputAction.MatchesTagExact(InputTag))
            {
                Info.InputTag = InputTag;
                AbilitiesInfo.AddUnique(Info);
            }
        }

    }
}

const FLevelDefaultInfo UVoidBlueprintFunctionLibrary::GetCurrentLevelInfo(UObject* WorldContextObject)
{
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr || VoidGameInstance->LevelDataAsset == nullptr)  return FLevelDefaultInfo();

    return VoidGameInstance->LevelDataAsset->LevelDefaultList[VoidGameInstance->CurrentLevelIndex];
}

int32 UVoidBlueprintFunctionLibrary::GetLastReachedLevel(UObject* WorldContextObject)
{
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr)  return 1.f;
    return VoidGameInstance->LastReachedLevel;
}

bool UVoidBlueprintFunctionLibrary::IsCanHitReact(const FGameplayEffectContextHandle& EffectContextHandle)
{
    const FVoidGameplayEffectContext* VoidEffectContext = static_cast<const FVoidGameplayEffectContext*>(EffectContextHandle.Get());
    if (VoidEffectContext)
    {
        return VoidEffectContext->IsCanHitReact();
    }
    return false;
}

bool UVoidBlueprintFunctionLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    const FVoidGameplayEffectContext* VoidEffectContext = static_cast<const FVoidGameplayEffectContext*>(EffectContextHandle.Get());
    if (VoidEffectContext)
    {
        return VoidEffectContext->IsBlockedHit();
    }
    return false;
}

bool UVoidBlueprintFunctionLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
    const FVoidGameplayEffectContext* VoidEffectContext = static_cast<const FVoidGameplayEffectContext*>(EffectContextHandle.Get());
    if (VoidEffectContext)
    {
        return VoidEffectContext->IsCriticalHit();
    }
    return false;
}

void UVoidBlueprintFunctionLibrary::SetCanHitReact(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool InCanHitReact)
{
    FVoidGameplayEffectContext* VoidEffectContext = static_cast<FVoidGameplayEffectContext*>(EffectContextHandle.Get());
    if (VoidEffectContext)
    {
        VoidEffectContext->SetCanHitReact(InCanHitReact);
    }
}

void UVoidBlueprintFunctionLibrary::SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInBlockedHit)
{
    FVoidGameplayEffectContext* VoidEffectContext = static_cast<FVoidGameplayEffectContext*>(EffectContextHandle.Get());
    if (VoidEffectContext)
    {
        VoidEffectContext->SetIsBlockedHit(bInBlockedHit);
    }
}

void UVoidBlueprintFunctionLibrary::SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInCriticalHit)
{
    FVoidGameplayEffectContext* VoidEffectContext = static_cast<FVoidGameplayEffectContext*>(EffectContextHandle.Get());
    if (VoidEffectContext)
    {
        VoidEffectContext->SetIsCriticalHit(bInCriticalHit);
    }
}

UWeaponInfo* UVoidBlueprintFunctionLibrary::GetWeaponClassInfo(UObject* WorldContextObject)
{
    AVoidGameModeBase* VoidGameMode = Cast<AVoidGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (VoidGameMode == nullptr) return nullptr;

    return VoidGameMode->WeaponClassInfo;
}

UAbilityInfo* UVoidBlueprintFunctionLibrary::GetAbilityInfo(UObject* WorldContextObject)
{
    AVoidGameModeBase* VoidGameMode = Cast<AVoidGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (VoidGameMode == nullptr) return nullptr;

    return VoidGameMode->AbilityInfo;
}

FGameplayTag UVoidBlueprintFunctionLibrary::GetGameplayCueByWeaponTag(const FGameplayTag& WeaponTag)
{
    for (const auto& Pair : FVoidGameplayTags::Get().WeaponTagtoCue)
    {
        if (Pair.Key.MatchesTagExact(WeaponTag))
        {
            return Pair.Value;
        }
    }
    return FGameplayTag();
}


void UVoidBlueprintFunctionLibrary::SpawnAndEquipWeapon(UObject* WorldContextObject, AVoidWeapon*& HeldWeapon, const FGameplayTag& WeaponTag, USkeletalMeshComponent* Mesh)
{
    if (!WorldContextObject || !Mesh) return;

    UWeaponInfo* WeaponClassInfo = GetWeaponClassInfo(WorldContextObject);
    if (WeaponClassInfo == nullptr) return;

    UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
    if (!World) return;
    
    FVoidWeaponInfo WeaponInfo = WeaponClassInfo->FindWeaponInfoByTag(WeaponTag);

    if (HeldWeapon)
    {
        HeldWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        HeldWeapon->SetOwner(nullptr);
        HeldWeapon->OnUnEquipWeapon();
        HeldWeapon = nullptr;
    }

    if (WeaponTag.MatchesTagExact(FVoidGameplayTags::Get().Weapon_None))
    {
        HeldWeapon = nullptr;
        return;
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    HeldWeapon = World->SpawnActor<AVoidWeapon>(WeaponInfo.WeaponClass, Params);

    if (HeldWeapon)
    {
        FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
        HeldWeapon->AttachToComponent(Mesh, TransformRules, WeaponInfo.WeaponSocketName);
        HeldWeapon->OnEquipWeapon();
    }

}

void UVoidBlueprintFunctionLibrary::GetPlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
    OutOverlappingActors.Empty();

    if (!WorldContextObject) return;

    UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
    if (!World) return;
       
    TArray<AActor*> OverlappedActors;

    UKismetSystemLibrary::SphereOverlapActors(
        WorldContextObject,
        SphereOrigin,
        Radius,
        TArray<TEnumAsByte<EObjectTypeQuery>>{UEngineTypes::ConvertToObjectType(ECC_Pawn)},
        AActor::StaticClass(),
        ActorsToIgnore,
        OverlappedActors
    );

    for (AActor* HitActor : OverlappedActors)
    {
        if (!HitActor)
            continue;

        if (HitActor->ActorHasTag(FName("Player")) &&
            HitActor->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()) &&
            !ICombatInterface::Execute_IsDead(HitActor))
        {
            OutOverlappingActors.Add(HitActor);
        }
    }
}

void UVoidBlueprintFunctionLibrary::GetEnemysWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
    OutOverlappingActors.Empty();

    if (!WorldContextObject) return;

    UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
    if (!World) return;

    TArray<AActor*> OverlappedActors;

    UKismetSystemLibrary::SphereOverlapActors(
        WorldContextObject,
        SphereOrigin,
        Radius,
        TArray<TEnumAsByte<EObjectTypeQuery>>{UEngineTypes::ConvertToObjectType(ECC_Pawn)},
        AActor::StaticClass(),
        ActorsToIgnore,
        OverlappedActors
    );

    for (AActor* HitActor : OverlappedActors)
    {
        if (!HitActor)
            continue;

        if (HitActor->ActorHasTag(FName("Enemy")) &&
            HitActor->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()) &&
            !ICombatInterface::Execute_IsDead(HitActor))
        {
            OutOverlappingActors.Add(HitActor);
        }
    }
}

FName UVoidBlueprintFunctionLibrary::GetKeyboardByInputActionTag(const UObject* WorldContextObject, const FGameplayTag& InputActionTag)
{
    if (!WorldContextObject) return NAME_None;

    AVoidGameModeBase* VoidGameMode = Cast<AVoidGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (!VoidGameMode || !VoidGameMode->InputConfig || !VoidGameMode->KeyDisplayData) return NAME_None;

    const UInputAction* InputAction = VoidGameMode->InputConfig->FindAbilityInputActionByTag(InputActionTag);
    if (!InputAction) return NAME_None;
    
    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    if (!PC) return NAME_None;
    
    const AVoidPlayerController* VoidPC = Cast<AVoidPlayerController>(PC);
    if (!VoidPC || !VoidPC->GetVoidContext())
        return NAME_None;

    const UInputMappingContext* Context = VoidPC->GetVoidContext();
   
    for (const FEnhancedActionKeyMapping& Mapping : Context->GetMappings())
    {
        if (Mapping.Action == InputAction)
        {
            return VoidGameMode->KeyDisplayData->GetDisplayNameForKey(Mapping.Key.GetFName());
        }
    }

    return NAME_None;

}

void UVoidBlueprintFunctionLibrary::AddHitImpluse(AActor* TargetActor, AActor* SourceActor, float ImpulseStrength, float ZImpluse)
{
    if (ACharacter* HitCharacter = Cast<ACharacter>(TargetActor))
    {
        if (!IsValid(HitCharacter) || HitCharacter->IsActorBeingDestroyed()) return;

        if (HitCharacter->GetCharacterMovement() && HitCharacter->GetMesh())
        {
            UCharacterMovementComponent* MoveComp = HitCharacter->GetCharacterMovement();
            FVector ImpulseDir = (HitCharacter->GetActorLocation() - SourceActor->GetActorLocation()).GetSafeNormal2D();
            ImpulseDir.Z += ZImpluse;
            ImpulseDir = ImpulseDir.GetSafeNormal();
            FVector Impulse = ImpulseDir * ImpulseStrength;

            if (HitCharacter->GetCharacterMovement() && !HitCharacter->GetMesh()->IsSimulatingPhysics())
            {
                HitCharacter->GetCharacterMovement()->AddImpulse(Impulse, true);
            }
        }
    }
}

void UVoidBlueprintFunctionLibrary::AddAttractionForce(AActor* TargetActor, AActor* SourceActor, float AttractionStrength, float ZForce)
{
    if (ACharacter* HitCharacter = Cast<ACharacter>(TargetActor))
    {
        FVector Direction = (SourceActor->GetActorLocation() - HitCharacter->GetActorLocation()).GetSafeNormal2D();
        Direction.Z += ZForce;
        Direction.Normalize();
        HitCharacter->LaunchCharacter(Direction * AttractionStrength, true, true);
    }
}

void UVoidBlueprintFunctionLibrary::AddAttractionForce(AActor* TargetActor, AActor* SourceActor, float AttractionStrength)
{
    if (ACharacter* HitCharacter = Cast<ACharacter>(TargetActor))
    {
        FVector Direction = (SourceActor->GetActorLocation() - HitCharacter->GetActorLocation()).GetSafeNormal();
        HitCharacter->LaunchCharacter(Direction * AttractionStrength, true, true);
    }
}

void UVoidBlueprintFunctionLibrary::AddAttractionForceWithRadius(AActor* TargetActor, AActor* SourceActor, float AttractionStrength, float Radius)
{
    if (ACharacter* HitCharacter = Cast<ACharacter>(TargetActor))
    {
        FVector Direction = (SourceActor->GetActorLocation() - HitCharacter->GetActorLocation()).GetSafeNormal();
        const float Distance = FVector::Dist(TargetActor->GetActorLocation(), SourceActor->GetActorLocation());
        if (Distance < Radius)
        {
            Direction *= (Distance / Radius);
        }
        HitCharacter->LaunchCharacter(Direction * AttractionStrength, true, true);
    }
}

int32 UVoidBlueprintFunctionLibrary::GetXPRewardForCharacterClassAndLevel(UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
    UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
    if (ClassInfo == nullptr) return 0;

    const FCharacterClassDefaultInfo Info = ClassInfo->GetCharacterDefaultInfo(CharacterClass);
    const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);
    return static_cast<int32>(XPReward);
}

void UVoidBlueprintFunctionLibrary::TempSaveCharacterProgress(ACharacter* Character, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(Character));
    if (VoidGameInstance == nullptr) return;

    AVoidPlayerState* VoidPlayerState = Cast<AVoidPlayerState>(PlayerState);
    UVoidAttributeSet* VoidAS = Cast<UVoidAttributeSet>(AttributeSet);
    UVoidAbilitySystemComponent* VoidASC = Cast<UVoidAbilitySystemComponent>(AbilitySystemComponent);

    if (!VoidPlayerState || !VoidAS || !VoidASC) return;

    VoidGameInstance->PlayerLevel = VoidPlayerState->GetPlayerLevel();
    VoidGameInstance->XP = VoidPlayerState->GetXP();

    VoidGameInstance->Health = VoidAS->GetHealth();
    VoidGameInstance->AddedMaxHealth = VoidAS->GetAddedMaxHealth();
    VoidGameInstance->MoveSpeedMultiplier = VoidAS->GetMoveSpeedMultiplier();
    VoidGameInstance->SprintSpeedMultiplier = VoidAS->GetSprintSpeedMultiplier();
    VoidGameInstance->HealthRegenRate = VoidAS->GetHealthRegenRate();
    VoidGameInstance->CriticalHitChance = VoidAS->GetCriticalHitChance();
    VoidGameInstance->CriticalHitMultiplier = VoidAS->GetCriticalHitMultiplier();
    VoidGameInstance->DamageReduction = VoidAS->GetDamageReduction();
    VoidGameInstance->BlockChance = VoidAS->GetBlockChance();
    VoidGameInstance->AttackSpeedMultiplier = VoidAS->GetAttackSpeedMultiplier();
    VoidGameInstance->MaxJumpCount = VoidAS->GetMaxJumpCount();

    VoidGameInstance->WeaponTag = IPlayerInterface::Execute_GetHeldWeaponTag(Character);
    VoidGameInstance->PickedupActors = VoidASC->PickedupActors;
    VoidGameInstance->IsInit = false;

    for (FGameplayAbilitySpec AbilitySpec : VoidASC->GetActivatableAbilities())
    {
        if (const UGameplayAbility* VoidAbility = Cast<UGameplayAbility>(AbilitySpec.Ability))
        {
            VoidGameInstance->AbilitiesTag.AddTag(VoidAbility->AbilityTags.First());
        }
    }
}

void UVoidBlueprintFunctionLibrary::LoadCharacterAttribute(ACharacter* Character, UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayEffect> DefaultLevel_SetByCaller, TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes, TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes_SetByCaller)
{
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(Character));

    FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
    ContextHandle.AddSourceObject(Character);
    FGameplayEffectSpecHandle SpecHandle_0 = AbilitySystemComponent->MakeOutgoingSpec(DefaultLevel_SetByCaller, 1.f, ContextHandle);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_0, FVoidGameplayTags::Get().Attribute_Primary_Level, VoidGameInstance->PlayerLevel);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle_0.Data.Get());

    FGameplayEffectSpecHandle SpecHandle_1 = AbilitySystemComponent->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle_1.Data.Get());

    FGameplayEffectSpecHandle SpecHandle_2 = AbilitySystemComponent->MakeOutgoingSpec(DefaultSecondaryAttributes_SetByCaller, 1.f, ContextHandle);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_Health, VoidGameInstance->Health);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_AddedMaxHealth, VoidGameInstance->AddedMaxHealth);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_MoveSpeedMultiplier, VoidGameInstance->MoveSpeedMultiplier);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_SprintSpeedMultiplier, VoidGameInstance->SprintSpeedMultiplier);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_HealthRegenRate, VoidGameInstance->HealthRegenRate);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_CriticalHitChance, VoidGameInstance->CriticalHitChance);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_CriticalHitMultiplier, VoidGameInstance->CriticalHitMultiplier);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_DamageReduction, VoidGameInstance->DamageReduction);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_BlockChance, VoidGameInstance->BlockChance);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_AttackSpeedMultiplier, VoidGameInstance->AttackSpeedMultiplier);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle_2, FVoidGameplayTags::Get().Attribute_Secondary_MaxJumpCount, VoidGameInstance->MaxJumpCount);
    AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle_2.Data.Get());
}

void UVoidBlueprintFunctionLibrary::LoadCharacterAbilties(ACharacter* Character, TArray<TSubclassOf<UGameplayAbility>>& OwnedAbilities)
{
    AVoidGameModeBase* VoidGameMode = Cast<AVoidGameModeBase>(UGameplayStatics::GetGameMode(Character));
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(Character));

    UAbilityInfo* AbilityInfo = VoidGameMode->AbilityInfo;

    for (const FGameplayTag& Tag : VoidGameInstance->AbilitiesTag)
    {
        OwnedAbilities.AddUnique(AbilityInfo->FindAbilityInfoByTag(Tag).AbilityClass);
    }
}


void UVoidBlueprintFunctionLibrary::SpawnDropItems(UObject* WorldContextObject, ECharacterClass CharacterClass, FVector SpawnLocation)
{
    UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
    if (!ClassInfo) return;

    FCharacterClassDefaultInfo DefaultInfo = ClassInfo->GetCharacterDefaultInfo(CharacterClass);
    const TArray<FDropItemsDefaultInfo>& ItemsInfo = DefaultInfo.DropItemsDefaultInfo;
    if (ItemsInfo.Num() == 0) return;

    float TotalProbability = 0.f;
    for (const FDropItemsDefaultInfo& ItemInfo : ItemsInfo)
    {
        TotalProbability += ItemInfo.DropProbability;
    }

    const float Randomfloat = FMath::FRandRange(0.f, 1.f);
    if (Randomfloat > TotalProbability) return;

    float Accumulated = 0.f;
    const FDropItemsDefaultInfo* SelectedDrop = nullptr;

    for (const FDropItemsDefaultInfo& Info : ItemsInfo)
    {
        Accumulated += Info.DropProbability;
        if (Randomfloat <= Accumulated)
        {
            SelectedDrop = &Info;
            break;
        }
    }

    if (SelectedDrop && SelectedDrop->ItemClass)
    {
        AVoidGameModeBase* VoidGameMode = Cast<AVoidGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
        if (VoidGameMode == nullptr) return;

        if (SelectedDrop->ItemType == EItemType::AbilityEffectItem && 
            (VoidGameMode->CurrentAbilityEffectItemNum >= ClassInfo->MaxAbilityEffectItemNum))
        {
            return;
        }
        if (SelectedDrop->ItemType == EItemType::AttributeEffectItem && 
            (VoidGameMode->CurrentAttributeEffectItemNum >= ClassInfo->MaxAttributeEffectItemNum))
        {
            return;
        }

        UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
        if (!World) return;
        FRotator SpawnRot = FRotator::ZeroRotator;
        World->SpawnActor<AVoidItem>(SelectedDrop->ItemClass, SpawnLocation, SpawnRot);
        
        if (SelectedDrop->ItemType == EItemType::AbilityEffectItem)
        {
            ++VoidGameMode->CurrentAbilityEffectItemNum;
        }
        if (SelectedDrop->ItemType == EItemType::AttributeEffectItem)
        {
            ++VoidGameMode->CurrentAttributeEffectItemNum;
        }
    }

}

void UVoidBlueprintFunctionLibrary::LoadNextLevel(UObject* WorldContextObject)
{
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr) return;

    ULevelInfo* LevelInfo = VoidGameInstance->LevelDataAsset;
    if (!LevelInfo || LevelInfo->LevelDefaultList.Num() <= 1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough levels to random pick!"));
        return;
    }

    const int32 MaxIndex = LevelInfo->LevelDefaultList.Num() - 1;
    const int32 RandomIndex = FMath::RandRange(1, MaxIndex);

    VoidGameInstance->CurrentLevelIndex = RandomIndex;

    VoidGameInstance->ShowLoadingScreen();

    const FLevelDefaultInfo& DefaultInfo = LevelInfo->LevelDefaultList[RandomIndex];

    if (!DefaultInfo.LevelRef.IsValid())
    {
        FSoftObjectPath LevelPath = DefaultInfo.LevelRef.ToSoftObjectPath();

        UAssetManager::GetStreamableManager().RequestAsyncLoad(
            LevelPath,
            FStreamableDelegate::CreateUObject(VoidGameInstance, &UVoidGameInstance::OnVoidLevelLoaded)
        );
    }
    else
    {
        VoidGameInstance->OnVoidLevelLoaded();
    }
}

void UVoidBlueprintFunctionLibrary::LoadLevelByIndex(UObject* WorldContextObject, int32 Index)
{
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr) return;

    ULevelInfo* LevelInfo = VoidGameInstance->LevelDataAsset;
    if (!LevelInfo || LevelInfo->LevelDefaultList.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No level data found!"));
        return;
    }

    if (Index > LevelInfo->LevelDefaultList.Num() - 1)
    {
        return;
    }
    VoidGameInstance->ShowLoadingScreen();
    const FLevelDefaultInfo& DefaultInfo = LevelInfo->LevelDefaultList[Index];
    VoidGameInstance->CurrentLevelIndex = Index;
    if (Index == 0)
    {
        VoidGameInstance->IsInit = true;
        VoidGameInstance->AbilitiesTag.Reset();
        VoidGameInstance->WeaponTag = FGameplayTag();
    }

    if (!DefaultInfo.LevelRef.IsValid())
    {
        FSoftObjectPath LevelPath = DefaultInfo.LevelRef.ToSoftObjectPath();

        UAssetManager::GetStreamableManager().RequestAsyncLoad(
            LevelPath,
            FStreamableDelegate::CreateUObject(VoidGameInstance, &UVoidGameInstance::OnVoidLevelLoaded)
        );
    }
    else
    {
        VoidGameInstance->OnVoidLevelLoaded();
    }
}

void UVoidBlueprintFunctionLibrary::BroadcastAbilityInfoOnPLayerDead(UAbilitySystemComponent* ASC)
{
    UVoidAbilitySystemComponent* VoidASC = Cast<UVoidAbilitySystemComponent>(ASC);
    if (!VoidASC) return;
    VoidASC->BroadcastAbilityInfo();
}

float UVoidBlueprintFunctionLibrary::GetAttackSpeedMultiplier(UAbilitySystemComponent* AbilitySystemComponent)
{
    if (!AbilitySystemComponent) return 1.f;

    const UVoidAttributeSet* VoidAS = AbilitySystemComponent->GetSet<UVoidAttributeSet>();
    const float Multiplier = FMath::Min<float>(4.f, VoidAS->GetAttackSpeedMultiplier());
    return Multiplier;
}

void UVoidBlueprintFunctionLibrary::PlayGlobalMusic(UObject* WorldContextObject, float VolumeMultiplier, float PitchMultiplier)
{
    if (GetCurrentLevelInfo(WorldContextObject).CycleSound.IsEmpty()) return;
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

    if (VoidGameInstance->GlobalMusicComp == nullptr)
    {
        UWorld* World = WorldContextObject->GetWorld();
        if (!World) return;

        AActor* MusicActor = World->SpawnActor<AActor>();
        MusicActor->SetActorHiddenInGame(true); // вўВи Actor

        VoidGameInstance->GlobalMusicComp = NewObject<UAudioComponent>(MusicActor);
        VoidGameInstance->GlobalMusicComp->bAutoActivate = false;
        VoidGameInstance->GlobalMusicComp->bIsUISound = true; 
        VoidGameInstance->GlobalMusicComp->RegisterComponent();  
    }

    if (VoidGameInstance->GlobalMusicComp->IsPlaying())
    {
        VoidGameInstance->GlobalMusicComp->FadeOut(0.5f, 0.0f);
    }
    TArray<USoundBase*> Sounds = GetCurrentLevelInfo(WorldContextObject).CycleSound;
    const int32 Index = FMath::RandRange(0, Sounds.Num() - 1);
    VoidGameInstance->GlobalMusicComp->SetSound(Sounds[Index]);
    VoidGameInstance->GlobalMusicComp->SetVolumeMultiplier(VolumeMultiplier);
    VoidGameInstance->GlobalMusicComp->SetPitchMultiplier(PitchMultiplier);
    VoidGameInstance->GlobalMusicComp->FadeIn(1.0f, VolumeMultiplier);
}

void UVoidBlueprintFunctionLibrary::StopGlobalMusic(UObject* WorldContextObject)
{
    if (WorldContextObject == nullptr) return;
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr || VoidGameInstance->GlobalMusicComp == nullptr) return;
    VoidGameInstance->GlobalMusicComp->FadeOut(1.f, 0.f);
}

void UVoidBlueprintFunctionLibrary::RecordStartTime(UObject* WorldContextObject)
{
    if (WorldContextObject == nullptr) return;
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr) return;
    VoidGameInstance->StartTime = FDateTime::Now();
    if (UWorld* World = WorldContextObject->GetWorld())
    {
        VoidGameInstance->GameStartTime = FPlatformTime::Seconds();
    }
}

void UVoidBlueprintFunctionLibrary::SavePlayHistory(UObject* WorldContextObject, const UAbilitySystemComponent* ASC, ECharacterClass KilledByCharacterClass, ACharacter* Player)
{
    if (WorldContextObject == nullptr || ASC == nullptr || Player == nullptr) return;
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr) return;

    UWorld* World = WorldContextObject->GetWorld();
    if (World == nullptr) return;

    ICombatInterface* CombatInterface = Cast<ICombatInterface>(Player);
    if (CombatInterface == nullptr) return;

    const UVoidAbilitySystemComponent* VoidASC = Cast<UVoidAbilitySystemComponent>(ASC);
    if (VoidASC == nullptr) return;

    FPlayHistory HistoryData;
    HistoryData.StartTime = VoidGameInstance->StartTime;
    HistoryData.PlayDuration = FPlatformTime::Seconds() - VoidGameInstance->GameStartTime;
    HistoryData.LastReachedLevel = VoidGameInstance->LastReachedLevel;
    HistoryData.PlayerLevel = CombatInterface->GetPlayerLevel();
    HistoryData.CharacterClass = KilledByCharacterClass;

    for (const FAttributeEffectActors& Actor : VoidASC->PickedupActors)
    {
        HistoryData.PickupActors.AddUnique(Actor);
    }
    for (const FGameplayAbilitySpec& AbilitySpec : VoidASC->GetActivatableAbilities())
    {

        const UGameplayAbility* Ability = Cast<UGameplayAbility>(AbilitySpec.Ability);
        if (Ability && Ability->AbilityTags.Num() > 0)
        {
            HistoryData.PlayerAbilityTag.AddTag(Ability->AbilityTags.First());
        }
    }

    const FString SlotName = TEXT("HistorySlot");
    const int32 UserIndex = 0;

    UVoidSaveGame* SaveObj = nullptr;

    if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
    {
        SaveObj = Cast<UVoidSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
    }

    if (!SaveObj)
    {
        SaveObj = Cast<UVoidSaveGame>(UGameplayStatics::CreateSaveGameObject(UVoidSaveGame::StaticClass()));
    }

    SaveObj->HistoryList.Add(HistoryData);

    UGameplayStatics::SaveGameToSlot(SaveObj, SlotName, UserIndex);

}

int32 UVoidBlueprintFunctionLibrary::LoadAllPlayHistory(UObject* WorldContextObject, TArray<FPlayHistory>& OutHistoryList)
{
    OutHistoryList.Empty();
    if (!WorldContextObject) return 0;

    const FString SlotName = TEXT("HistorySlot");
    const int32 UserIndex = 0;

    if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
    {
        return 0;
    }

    UVoidSaveGame* SaveObj = Cast<UVoidSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
    if (!SaveObj)
    {
        return 0;
    }

    OutHistoryList = SaveObj->HistoryList;
    return SaveObj->HistoryList.Num();
}

FText UVoidBlueprintFunctionLibrary::DateTimeToText(const FDateTime& DateTime)
{
    const FString Formatted = DateTime.ToString(TEXT("%Y-%m-%d %H:%M:%S"));
    return FText::FromString(Formatted);
}

FText UVoidBlueprintFunctionLibrary::FormatSecondsToText(float Seconds)
{
    Seconds = FMath::Max(0.f, Seconds);

    const int32 TotalSeconds = FMath::FloorToInt(Seconds);

    const int32 Minutes = TotalSeconds / 60;
    const int32 Sec = TotalSeconds % 60;

    const FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Sec);

    return FText::FromString(TimeString);
}

void UVoidBlueprintFunctionLibrary::SavePlayerStartupAbilities(UObject* WorldContextObject, const FGameplayTag AbilityTag, const FGameplayTag InputTag)
{
    if (WorldContextObject == nullptr) return;
    UVoidGameInstance* VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (VoidGameInstance == nullptr) return;
    if (InputTag.MatchesTagExact(FVoidGameplayTags::Get().Input_Skill01))
    {
        VoidGameInstance->WeaponTag = FVoidGameplayTags::Get().AbilityTagToWeapon.FindChecked(AbilityTag);
        return;
    }
    VoidGameInstance->AbilitiesTag.AddTag(AbilityTag);
}





