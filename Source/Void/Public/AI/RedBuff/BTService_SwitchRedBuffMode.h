// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_SwitchRedBuffMode.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API UBTService_SwitchRedBuffMode : public UBTService_BlueprintBase
{
	GENERATED_BODY()

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector RedBuffModeSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;

	UPROPERTY(EditAnywhere, Category = "AI")
	float ChaseDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float ValidDistance = 4000.f;
	
};
