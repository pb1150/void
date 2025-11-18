// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/VoidHUD.h"
#include "Player/VoidPlayerState.h"
#include "Player/VoidPlayerController.h"
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
    if (!LevelInfo || LevelInfo->LevelDefaultList.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No level data found!"));
        return;
    }

    if (VoidGameInstance->CurrentLevelIndex > LevelInfo->LevelDefaultList.Num() - 2)
    {
        UE_LOG(LogTemp, Log, TEXT("All levels cleared!"));
        return;
    }

    VoidGameInstance->CurrentLevelIndex++;

    VoidGameInstance->ShowLoadingScreen();
    const FLevelDefaultInfo& DefaultInfo = LevelInfo->LevelDefaultList[VoidGameInstance->CurrentLevelIndex];
   
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





