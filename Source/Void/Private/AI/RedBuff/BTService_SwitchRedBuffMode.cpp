// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RedBuff/BTService_SwitchRedBuffMode.h"
#include "AIController.h"
#include "Character/Enemy/VoidEnemy_Buff.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UBTService_SwitchRedBuffMode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();

	AVoidEnemy_Buff* RedBuff = Cast<AVoidEnemy_Buff>(OwningPawn);
	if (!RedBuff) return;

	float Distance = UBTFunctionLibrary::GetBlackboardValueAsFloat(this, DistanceToTargetSelector);
	
	if (Distance >= ValidDistance)
	{
		UBTFunctionLibrary::SetBlackboardValueAsEnum(this, RedBuffModeSelector, static_cast<int32>(ERedBuffState::Idle));
	}
	else if (Distance >= ChaseDistance && Distance < ValidDistance)
	{
		UBTFunctionLibrary::SetBlackboardValueAsEnum(this, RedBuffModeSelector, static_cast<int32>(ERedBuffState::Chase));
	}
	else
	{
		const float Rand = FMath::FRand();
		if (Rand < 0.3f)
		{
			UBTFunctionLibrary::SetBlackboardValueAsEnum(this, RedBuffModeSelector, static_cast<int32>(ERedBuffState::MeleeAttack));
		}
		else if (Rand < 0.55f)
		{
			UBTFunctionLibrary::SetBlackboardValueAsEnum(this, RedBuffModeSelector, static_cast<int32>(ERedBuffState::Uppercut));
		}
		else if(Rand < 0.85f)
		{
			UBTFunctionLibrary::SetBlackboardValueAsEnum(this, RedBuffModeSelector, static_cast<int32>(ERedBuffState::BigSmash));
		}
		else
		{
			UBTFunctionLibrary::SetBlackboardValueAsEnum(this, RedBuffModeSelector, static_cast<int32>(ERedBuffState::Confront));
		}

	}

}
