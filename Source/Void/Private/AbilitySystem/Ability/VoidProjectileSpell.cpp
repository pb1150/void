// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/VoidProjectileSpell.h"
#include "Item/VoidProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "VoidGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void UVoidProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);
}

void UVoidProjectileSpell::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UVoidProjectileSpell::SpawnProjectile(const APlayerController* PlayerController)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SpawnLocation = ICombatInterface::Execute_GetProjectileSocketLocation_1(GetAvatarActorFromActorInfo());
		const FRotator SpawnRotation = PlayerController->GetControlRotation();

		SpawnActorByTransform(SpawnLocation, SpawnRotation, true);
	}

}

void UVoidProjectileSpell::SpawnRifleProjectile(const APlayerController* PlayerController)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SpawnLocation = ICombatInterface::Execute_GetRifleBulletSocketLocation_1(GetAvatarActorFromActorInfo());
		const FRotator SpawnRotation = ICombatInterface::Execute_GetRifleBulletSocketRotation_1(GetAvatarActorFromActorInfo());

		SpawnActorByTransform(SpawnLocation, SpawnRotation, false);
	}
}

void UVoidProjectileSpell::SpawnLightning()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorRotation();

	AVoidProjectile* Projectile = SpawnActorByTransform(SpawnLocation, SpawnRotation, false);
	Projectile->OnDestroyed.AddDynamic(this, &UVoidProjectileSpell::OnProjectileDestroyed);
	Projectile->AttachToActor(GetAvatarActorFromActorInfo(), FAttachmentTransformRules::KeepWorldTransform);
}

void UVoidProjectileSpell::SpawnArea()
{
	const FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FRotator SpawnRotation = GetAvatarActorFromActorInfo()->GetActorRotation();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	AActor* Projectile = GetWorld()->SpawnActorDeferred<AActor>(ActorClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->SetOwner(GetAvatarActorFromActorInfo());

	Projectile->FinishSpawning(SpawnTransform);
	Projectile->OnDestroyed.AddDynamic(this, &UVoidProjectileSpell::OnProjectileDestroyed);
}

void UVoidProjectileSpell::SpawnTornado(const APlayerController* PlayerController)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + (GetAvatarActorFromActorInfo()->GetActorForwardVector() * 50.f);
		FRotator ControlRot = PlayerController->GetControlRotation();
		FRotator SpawnRotation(0.f, ControlRot.Yaw, 0.f);

		AVoidProjectile* Projectile = SpawnActorByTransform(SpawnLocation, SpawnRotation, false);
		Projectile->OnDestroyed.AddDynamic(this, &UVoidProjectileSpell::OnProjectileDestroyed);
	}
}

void UVoidProjectileSpell::SpawnBlackHole(const APlayerController* PlayerController)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer || !PlayerController) return;

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector Start = ViewLocation;
	FVector End = Start + ViewRotation.Vector() * 1000.f;

	FVector HalfSize(50.f, 50.f, 50.f);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	FHitResult Hit;

	bool bHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start,
		End,
		HalfSize,
		ViewRotation,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true
	);

	FVector SpawnLocation = bHit ? Hit.Location : End;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	AVoidProjectile* Projectile = SpawnActorByTransform(SpawnLocation, SpawnRotation, false);
	Projectile->OnDestroyed.AddDynamic(this, &UVoidProjectileSpell::OnProjectileDestroyed);

}


AVoidProjectile* UVoidProjectileSpell::SpawnActorByTransform(FVector SpawnLocation, FRotator SpawnRotation, bool CanHitReact)
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	AVoidProjectile* Projectile = GetWorld()->SpawnActorDeferred<AVoidProjectile>(ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (IsValid(DamageEffectClass))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		UVoidBlueprintFunctionLibrary::SetCanHitReact(EffectContextHandle, CanHitReact);

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FVoidGameplayTags::Get().Damage, PlayerDamageMultiplier);
		Projectile->DamageEffectSpecHandle = SpecHandle;
	}
	Projectile->SetOwner(GetAvatarActorFromActorInfo());

	Projectile->FinishSpawning(SpawnTransform);
	return Projectile;
}

void UVoidProjectileSpell::OnProjectileDestroyed(AActor* DestroyedActor)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), CurrentActivationInfo, true, false);
}

