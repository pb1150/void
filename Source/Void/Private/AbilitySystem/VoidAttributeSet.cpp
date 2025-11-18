// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/VoidAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "VoidGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/VoidPlayerController.h"
#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"



UVoidAttributeSet::UVoidAttributeSet()
{
	FVoidGameplayTags GameplayTags = FVoidGameplayTags::Get();

	TagstoAttributes.Add(GameplayTags.Attribute_Primary_Damage, GetDamageAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Primary_Level, GetLevelAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Primary_MaxHealth, GetMaxHealthAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_AddedMaxHealth, GetAddedMaxHealthAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_BlockChance, GetBlockChanceAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_CriticalHitMultiplier, GetCriticalHitMultiplierAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_DamageReduction, GetDamageReductionAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_Health, GetHealthAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_HealthRegenRate, GetHealthRegenRateAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_MoveSpeedMultiplier, GetMoveSpeedMultiplierAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_SprintSpeedMultiplier, GetSprintSpeedMultiplierAttribute());
	TagstoAttributes.Add(GameplayTags.Attribute_Secondary_AttackSpeedMultiplier, GetAttackSpeedMultiplierAttribute());

}

void UVoidAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/** Primary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, Damage, COND_None, REPNOTIFY_Always);

	/** Secondary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, AddedMaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, MoveSpeedMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, SprintSpeedMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, CriticalHitMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, DamageReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, AttackSpeedMultiplier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UVoidAttributeSet, MaxJumpCount, COND_None, REPNOTIFY_Always);
}

void UVoidAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UVoidAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0.f;
			if (bFatal)
			{
				SendXPEvent(Props);
				
				if (Props.TargetCharacter->Implements<UPlayerInterface>())
				{
					if (Props.SourceCharacter->Implements<UEnemyInterface>())
					{
						ECharacterClass EnemyClass = IEnemyInterface::Execute_GetCharacterClass(Props.SourceCharacter);
						OnPlayerDead.ExecuteIfBound(EnemyClass);
					}
					else
					{
						OnPlayerDead.ExecuteIfBound(ECharacterClass::Unknown);
					}
					UVoidBlueprintFunctionLibrary::BroadcastAbilityInfoOnPLayerDead(Props.TargetASC);
				}

				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}


			}
			else
			{
				if (UVoidBlueprintFunctionLibrary::IsCanHitReact(Props.EffectContextHandle))
				{
					for (FGameplayAbilitySpec& Spec : Props.TargetASC->GetActivatableAbilities())
					{
						if (Spec.Ability && Spec.Ability->AbilityTags.HasTagExact(FVoidGameplayTags::Get().Effect_HitReact))
						{
							Props.TargetASC->TryActivateAbility(Spec.Handle);

							FGameplayEventData EventData;
							EventData.EventTag = FVoidGameplayTags::Get().Effect_HitReact;
							EventData.Target = Props.TargetCharacter;
							EventData.ContextHandle = Props.EffectContextHandle;

							UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
								Props.TargetCharacter,
								FVoidGameplayTags::Get().Effect_HitReact,
								EventData
							);
							break;
						}
					}
				}
			}

			const bool bBlocked = UVoidBlueprintFunctionLibrary::IsBlockedHit(Props.EffectContextHandle);
			const bool bCritical = UVoidBlueprintFunctionLibrary::IsCriticalHit(Props.EffectContextHandle);
			ShowFloatingText(Props, LocalIncomingDamage, bBlocked, bCritical);


		}

	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		const float LocalIncomingXP = GetIncomingXP();
		SetIncomingXP(0.f);
		UE_LOG(LogTemp, Warning, TEXT("Incoming XP: %f"), LocalIncomingXP);

		//SourceCharacter is Player GA_ListenForEvents Applies GE_EventBasedEffect, Adding to IncomingXP
		if (Props.SourceCharacter->Implements<UPlayerInterface>())
		{
			const int32 CurrentLevel = GetLevel();
			const int32 CurrentXP = IPlayerInterface::Execute_GetXp(Props.SourceCharacter);
			const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXp(Props.SourceCharacter, LocalIncomingXP + CurrentXP);

			const int32 NumLevelUps = NewLevel - CurrentLevel;
			if (NumLevelUps > 0)
			{
				IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
				IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
				SetLevel(NewLevel);
			}

			IPlayerInterface::Execute_AddToXp(Props.SourceCharacter, LocalIncomingXP);
		}
	}

}


void UVoidAttributeSet::ShowFloatingText(const FEffectProperties& Props, float LocalIncomingDamage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		AVoidPlayerController* PC = Cast<AVoidPlayerController>(Props.SourceController);
		if (PC)
		{
			PC->ShowDamageNumber(LocalIncomingDamage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}

	}
}

void UVoidAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(Props.TargetAvatarActor);
	if (EnemyInterface == nullptr) return;

	const int32 XPReward = UVoidBlueprintFunctionLibrary::GetXPRewardForCharacterClassAndLevel(Props.TargetAvatarActor, IEnemyInterface::Execute_GetCharacterClass(Props.TargetAvatarActor), GetLevel());
	 
	FGameplayEventData EventData;
	EventData.EventTag = FVoidGameplayTags::Get().Attribute_Meta_IncomingXP;
	EventData.EventMagnitude = static_cast<float>(XPReward);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Props.SourceCharacter,
		FVoidGameplayTags::Get().Attribute_Meta_IncomingXP,
		EventData
	);

}



void UVoidAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	Props.EffectContextHandle = EffectContextHandle;
	UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	Props.SourceASC = SourceASC;

	if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* SourceAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
		AController* SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
			{
				SourceController = Pawn->GetController();
			}
		}
		if (SourceController)
		{
			ACharacter* SourceCharacter = Cast<ACharacter>(SourceController->GetPawn());
			Props.SourceCharacter = SourceCharacter;
		}
		Props.SourceAvatarActor = SourceAvatarActor;
		Props.SourceController = SourceController;
	}


	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		ACharacter* TargetCharacter = Cast<ACharacter>(TargetAvatarActor);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetAvatarActor);

		Props.TargetASC = TargetASC;
		Props.TargetAvatarActor = TargetAvatarActor;
		Props.TargetCharacter = TargetCharacter;
		Props.TargetController = TargetController;
	}
}

void UVoidAttributeSet::OnRep_Level(const FGameplayAttributeData& OldLevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, Level, OldLevel);
}

void UVoidAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, Health, OldHealth);
}

void UVoidAttributeSet::OnRep_AddedMaxHealth(const FGameplayAttributeData& OldAddedMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, AddedMaxHealth, OldAddedMaxHealth);
}

void UVoidAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, MaxHealth, OldMaxHealth);
}

void UVoidAttributeSet::OnRep_Damage(const FGameplayAttributeData& OldDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, Damage, OldDamage);
}

void UVoidAttributeSet::OnRep_MoveSpeedMultiplier(const FGameplayAttributeData& OldMoveSpeedMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, MoveSpeedMultiplier, OldMoveSpeedMultiplier);
}

void UVoidAttributeSet::OnRep_SprintSpeedMultiplier(const FGameplayAttributeData& OldSprintSpeedMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, SprintSpeedMultiplier, OldSprintSpeedMultiplier);
}

void UVoidAttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, HealthRegenRate, OldHealthRegenRate);
}

void UVoidAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UVoidAttributeSet::OnRep_CriticalHitMultiplier(const FGameplayAttributeData& OldCriticalHitMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, CriticalHitMultiplier, OldCriticalHitMultiplier);
}

void UVoidAttributeSet::OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, DamageReduction, OldDamageReduction);
}

void UVoidAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, BlockChance, OldBlockChance);
}

void UVoidAttributeSet::OnRep_AttackSpeedMultiplier(const FGameplayAttributeData& OldAttackSpeedMultiplier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, AttackSpeedMultiplier, OldAttackSpeedMultiplier);
}

void UVoidAttributeSet::OnRep_MaxJumpCount(const FGameplayAttributeData& OldMaxJumpCount) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UVoidAttributeSet, MaxJumpCount, OldMaxJumpCount);
}


