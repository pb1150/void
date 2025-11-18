// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return;

	APawn* OwningPawn = AI->GetPawn();
	if (!OwningPawn) return;

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, Actors);

	float MixDistance = TNumericLimits<float>::Max();
	AActor* NearestActor = nullptr;

	for (AActor* Actor : Actors)
	{
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = FVector::Dist(Actor->GetActorLocation(), OwningPawn->GetActorLocation());
			if (Distance < MixDistance)
			{
				MixDistance = Distance;
				NearestActor = Actor;
			}
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetActorSelector, NearestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, MixDistance);


	/* Find PlayerLocation Dynamic */

	if (!NearestActor)
	{
		UBTFunctionLibrary::SetBlackboardValueAsVector(this, PlayerLocationSelector, FVector::ZeroVector);
		return;
	}

	const FVector TargetLoc = NearestActor->GetActorLocation();
	FVector FinalLocation = TargetLoc;

	bool bInAir = false;

	if (ACharacter* Character = Cast<ACharacter>(NearestActor))
	{
		if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
		{
			bInAir = !MovementComponent->IsMovingOnGround();
		}
	}

	if (!bInAir)
	{
		UBTFunctionLibrary::SetBlackboardValueAsVector(this, PlayerLocationSelector, TargetLoc);
		return;
	}

	UWorld* World = OwnerComp.GetWorld();
	if (!World)
	{
		UBTFunctionLibrary::SetBlackboardValueAsVector(this, PlayerLocationSelector, TargetLoc);
		return;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World);
	if (!NavSys)
	{
		UBTFunctionLibrary::SetBlackboardValueAsVector(this, PlayerLocationSelector, TargetLoc);
		return;
	}

	FNavLocation ProjectedLoc;
	if (NavSys->ProjectPointToNavigation(TargetLoc, ProjectedLoc, QueryExtent))
	{
		FinalLocation = ProjectedLoc.Location;
	}

	UBTFunctionLibrary::SetBlackboardValueAsVector(this, PlayerLocationSelector, FinalLocation);
}
