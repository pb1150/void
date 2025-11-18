// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VoidEnemy.h"
#include "VoidEnemy_BlackBuff.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class VOID_API AVoidEnemy_BlackBuff : public AVoidEnemy
{
	GENERATED_BODY()

public:
	AVoidEnemy_BlackBuff();

	/** Combat Interface */
	virtual void Die() override;

	/** End Combat Interface */

	/** Enemy Interface */
	virtual FVector GetSpawnActorLocation_Implementation() override;

	/** End Enemy Interface */


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Dissolve() override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;
	
};
