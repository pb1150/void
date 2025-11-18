// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/VoidShootCable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "CableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UVoidShootCable::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false);

    AActor* Avatar = GetAvatarActorFromActorInfo();
    if (!IsValid(Avatar)) return;

	APlayerController* PC = Cast<APlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController());
	if (PC == nullptr) return;

	FVector ViewLocation;
	FRotator ViewRotation;
	PC->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FVector Start = ViewLocation;
	FVector End = Start + ViewRotation.Vector() * ValidDistance;

	FVector HalfSize(2.f, 2.f, 2.f);

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
	
    UCableComponent* Cable = nullptr;
    USceneComponent* CableTarget = nullptr;

    if (Avatar->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
    {
        CableTarget = IPlayerInterface::Execute_GetCableTargetComponent(Avatar);
        Cable = IPlayerInterface::Execute_GetCableComponent(Avatar);
    }

    if (!IsValid(CableTarget) || !IsValid(Cable))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    FVector TargetLocation = bHit ? Hit.Location : End;

    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(CableShootTimer);
    GetWorld()->GetTimerManager().ClearTimer(CableRetractTimer);
    GetWorld()->GetTimerManager().ClearTimer(CableForceEndTimer);
    Cable->SetVisibility(true);

    FTimerDelegate ShootDel;
    ShootDel.BindUObject(this, &UVoidShootCable::CableShootTick, CableTarget, TargetLocation, bHit);
    World->GetTimerManager().SetTimer(CableShootTimer, ShootDel, 0.01f, true);

    FTimerDelegate EndDel;
    EndDel.BindUObject(this, &UVoidShootCable::ForceEndAbility, CableTarget, Cable);
    World->GetTimerManager().SetTimer(CableForceEndTimer, EndDel, MaxAbilityDuration, false);

}

void UVoidShootCable::CableShootTick(USceneComponent* CableTarget, FVector TargetLocation, bool bHit)
{
    UWorld* World = GetWorld();
    if (!IsValid(World) || !IsValid(CableTarget))
    {
        if (IsValid(World))
        {
            World->GetTimerManager().ClearTimer(CableShootTimer);
        }
        return;
    }

    const float Dt = World->GetDeltaSeconds();

    const FVector StartLocation = CableTarget->GetComponentLocation();
    const FVector NewLocation = FMath::VInterpConstantTo(StartLocation, TargetLocation, Dt, CableSpeed);
    CableTarget->SetWorldLocation(NewLocation);

    const float DistRemaining = FVector::Dist(TargetLocation, NewLocation);

    if (DistRemaining <= (CableAcceptableRadius + CableSpeed * Dt))
    {
        CableTarget->SetWorldLocation(TargetLocation);
        World->GetTimerManager().ClearTimer(CableShootTimer);
        StartCableRetract(CableTarget, TargetLocation, bHit);
    }
}

void UVoidShootCable::StartCableRetract(USceneComponent* CableTarget, const FVector& TargetLocation, bool bHit)
{
    AActor* Avatar = GetAvatarActorFromActorInfo();
    ACharacter* Character = Cast<ACharacter>(Avatar);
    ElapsedDist = 0.f;

    UCableComponent* Cable = nullptr;
    if (IsValid(Avatar) && Avatar->GetClass()->ImplementsInterface(UPlayerInterface::StaticClass()))
    {
        Cable = IPlayerInterface::Execute_GetCableComponent(Avatar);
    }

    if (!IsValid(CableTarget) || !IsValid(Cable) || !IsValid(Character))
    {
        UWorld* World = GetWorld();
        if (IsValid(World))
        {
            World->GetTimerManager().ClearTimer(CableRetractTimer);
        }
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), CurrentActivationInfo, true, true);
        return;
    }

    float InitialDisToTarget = 0.f;
    if (bHit)
    {
        InitialDisToTarget = FVector::Dist(Character->GetActorLocation(), TargetLocation);
        
        if (InitialDisToTarget < KINDA_SMALL_NUMBER)
        {
            InitialDisToTarget = 1.f;
        }
    }

    UWorld* World = GetWorld();
    if (!IsValid(World))
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), CurrentActivationInfo, true, true);
        return;
    }

    TWeakObjectPtr<UCableComponent> CableWeak(Cable);
    TWeakObjectPtr<ACharacter>      CharacterWeak(Character);

    FTimerDelegate RetractDel;
    RetractDel.BindUObject(this, &UVoidShootCable::CableRetractTick, CableTarget, TargetLocation, bHit, CableWeak, CharacterWeak, InitialDisToTarget);
    World->GetTimerManager().SetTimer(CableRetractTimer, RetractDel, 0.01f, true);
}

void UVoidShootCable::ForceEndAbility(USceneComponent* CableTarget, UCableComponent* Cable)
{
    if (CableTarget && Cable)
    {
        Cable->SetVisibility(false);
        CableTarget->SetWorldLocation(Cable->GetComponentLocation());
    }
    if (IsActive())
    {
        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), CurrentActivationInfo, true, false);
    }
}

void UVoidShootCable::CableRetractTick(USceneComponent* CableTarget, FVector TargetLocation, bool bHit, TWeakObjectPtr<class UCableComponent> CableWeak, TWeakObjectPtr<class ACharacter> CharacterWeak, float InitialDisToTarget)
{
    UWorld* World = GetWorld();
    if (!IsValid(World) || !IsValid(CableTarget) || !CableWeak.IsValid() || !CharacterWeak.IsValid())
    {
        if (IsValid(World))
        {
            World->GetTimerManager().ClearTimer(CableRetractTimer);
        }
        return;
    }

    UCableComponent* Cable = CableWeak.Get();
    ACharacter* Character = CharacterWeak.Get();

    const float Dt = World->GetDeltaSeconds();
    const FVector CurrentCableStart = Cable->GetComponentLocation();

    if (bHit)
    {
        CableTarget->SetWorldLocation(TargetLocation);

        const FVector CharLoc = Character->GetActorLocation();
        const float   Dist = FVector::Dist(CharLoc, TargetLocation);

        const float SafeInitial = FMath::Max(InitialDisToTarget, 1.f);
        const float SpeedPercent = FMath::Clamp(Dist / SafeInitial, 0.f, 1.f);

        const FVector PullDir = (TargetLocation - CharLoc).GetSafeNormal();
        const FVector LaunchVel = PullDir * PullSpeed * SpeedPercent;

        if (LaunchVel.ContainsNaN() == false)
        {
            Character->LaunchCharacter(LaunchVel, true, true);
        }
    }
    else
    {
        const float Step = CableSpeed * Dt;
        ElapsedDist += Step;

        const FVector BackDir = (CurrentCableStart - TargetLocation).GetSafeNormal();
        const FVector NewCableEnd = TargetLocation + BackDir * ElapsedDist;

        CableTarget->SetWorldLocation(NewCableEnd);
    }

    const FVector CheckPoint = bHit ? TargetLocation : CableTarget->GetComponentLocation();
    const float Remaining = FVector::Dist(CheckPoint, CurrentCableStart);

    if (Remaining <= StopTolerance)
    {
        if (bHit)
        {
            Character->LaunchCharacter(FVector::UpVector * UpwardSpeed, true, true);
        }

        CableTarget->SetWorldLocation(CurrentCableStart);
        Cable->SetVisibility(false);

        World->GetTimerManager().ClearTimer(CableRetractTimer);

        EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), CurrentActivationInfo, true, false);
    }
}


void UVoidShootCable::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(CableShootTimer);
        World->GetTimerManager().ClearTimer(CableRetractTimer);
        World->GetTimerManager().ClearTimer(CableForceEndTimer);
    }
}

void UVoidShootCable::BeginDestroy()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearAllTimersForObject(this);
    }
    Super::BeginDestroy();
}
