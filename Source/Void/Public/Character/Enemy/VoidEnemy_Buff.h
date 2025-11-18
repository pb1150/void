// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VoidEnemy.h"
#include "VoidEnemy_Buff.generated.h"

class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class ERedBuffState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),        // 静止状态
	Chase			UMETA(DisplayName = "ChasetoMelee"),
	Confront        UMETA(DisplayName = "Confront"),    // 对峙状态
	MeleeAttack     UMETA(DisplayName = "MeleeAttack"), // 近战攻击
	Uppercut		UMETA(DisplayName = "Uppercut"),      // 技能1
	BigSmash		UMETA(DisplayName = "BigSmash")       // 技能2
};

/**
 * 
 */
UCLASS()
class VOID_API AVoidEnemy_Buff : public AVoidEnemy
{
	GENERATED_BODY()

public:
	AVoidEnemy_Buff();

	/** Combat Interface */
	virtual void Die() override;

	/** End Combat Interface */

	/** Enemy Interface */
	virtual FVector GetSpawnActorLocation_Implementation() override;
	virtual FVector GetWarpingLocation_Implementation() override;
	/** End Enemy Interface */

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Dissolve() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float BigSmashDistanceToTarget = 180.f;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

};
