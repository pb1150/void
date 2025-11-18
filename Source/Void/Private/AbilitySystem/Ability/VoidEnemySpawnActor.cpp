// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/VoidEnemySpawnActor.h"
#include "Interaction/EnemyInterface.h"
#include "Item/VoidEffectItem.h"
#include "Item/VoidProjectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "VoidGameplayTags.h"

void UVoidEnemySpawnActor::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UVoidEnemySpawnActor::SpawnActor()
{
	if (!IsValid(ActorClass)) return;
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !Avatar->Implements<UEnemyInterface>()) return;

	 
	FVector SpawnLocation = IEnemyInterface::Execute_GetSpawnActorLocation(Avatar);
	FRotator SpawnRotation = Avatar->GetActorRotation();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	AVoidEffectItem* EffectActor = GetWorld()->SpawnActorDeferred<AVoidEffectItem>(ActorClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Avatar);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(EffectActor);
	EffectContextHandle.AddActors(Actors);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FVoidGameplayTags::Get().Damage, GetDamageMultiplierFromCharacterClass());
	EffectActor->DamageEffectSpecHandle = SpecHandle;

	EffectActor->FinishSpawning(SpawnTransform);

}

void UVoidEnemySpawnActor::SpawnProjectileToCombatTarget()
{
	if (!IsValid(ProjectileClass)) return;
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !Avatar->Implements<UEnemyInterface>()) return;

	FVector SpawnLocation = IEnemyInterface::Execute_GetSpawnActorLocation(Avatar);
	FRotator SpawnRotation = Avatar->GetActorRotation();

	if (IsValid(IEnemyInterface::Execute_GetCombatTarget(Avatar)))
	{
		FVector CombatTargetLocation = IEnemyInterface::Execute_GetCombatTarget(Avatar)->GetActorLocation();
		SpawnRotation = (CombatTargetLocation - SpawnLocation).Rotation();
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	
	AVoidProjectile* Projectile = GetWorld()->SpawnActorDeferred<AVoidProjectile>(ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Avatar);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FVoidGameplayTags::Get().Damage, GetDamageMultiplierFromCharacterClass());
	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->SetOwner(Avatar);

	Projectile->FinishSpawning(SpawnTransform);

}
