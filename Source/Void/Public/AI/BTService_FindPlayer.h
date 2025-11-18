// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API UBTService_FindPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()
	
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetActorSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	FBlackboardKeySelector PlayerLocationSelector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector QueryExtent = FVector(200.f, 200.f, 1000.f); //ËÑË÷ºÐ×ÓµÄÒ»°ë³ß´ç
};
