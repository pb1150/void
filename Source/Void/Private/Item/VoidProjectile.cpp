// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/VoidProjectile.h"
#include "Void/Void.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"



AVoidProjectile::AVoidProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	SetRootComponent(BoxCollision);

	BoxCollision->SetCollisionObjectType(ECC_Projectile);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 1800.f;
	ProjectileMovement->MaxSpeed = 1800.f;
	ProjectileMovement->ProjectileGravityScale = 0.4f;
}


void AVoidProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	CurrentLoopCount = 0;

	if (BoxCollision && CanUseBoxCollision)
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AVoidProjectile::OnBeginOverlay);
	}

}

void AVoidProjectile::Destroyed()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearAllTimersForObject(this);
	}
	Super::Destroyed();
}

void AVoidProjectile::OnBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() == OtherActor) return;
	if (OtherActor->ActorHasTag("Enemy") && !CanHitEnemy) return;

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	}

	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation(), FRotator::ZeroRotator, true);
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (TargetASC && DamageEffectSpecHandle.IsValid())
	{
		DamageEffectSpecHandle.Data.Get()->GetContext().AddHitResult(SweepResult);
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		if (CanHitImpluse)
		{
			UVoidBlueprintFunctionLibrary::AddHitImpluse(OtherActor, GetOwner(), 400.f);
		} 
	}

	if (bSimulatePhysics)
	{
		ProjectileMovement->Deactivate();

		BoxCollision->SetGenerateOverlapEvents(false);
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		BoxCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		BoxCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		BoxCollision->SetSimulatePhysics(true);
	}
	else
	{
		Destroy();
	}
}

void AVoidProjectile::StartTimerDamage()
{
	UWorld* World = GetWorld();
	if (!World) return;
	FTimerDelegate FTimerDelegate;
	FTimerDelegate.BindUObject(this, &AVoidProjectile::DetectionAndApplyDamageByEffectSpecHandle);
	World->GetTimerManager().SetTimer(ProjectileTimer, FTimerDelegate, TimerInterval, true);
}

void AVoidProjectile::DetectionAndApplyDamageByEffectSpecHandle()
{
	TArray<AActor*> OutOverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UVoidBlueprintFunctionLibrary::GetEnemysWithinRadius(this, OutOverlappingActors, ActorsToIgnore, DamageRadius, GetActorLocation());

	for (AActor* Actor : OutOverlappingActors)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
		if (ASC && DamageEffectSpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		if (bForce)
		{
			UVoidBlueprintFunctionLibrary::AddAttractionForceWithRadius(Actor, this, ForceStrength, DamageRadius);
		}
	}

	CurrentLoopCount++;
	if (CurrentLoopCount >= MaxLoopCount)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearAllTimersForObject(this);
			Destroy();
			return;
		}
	}
}






