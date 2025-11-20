// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VoidEnemy.h"
#include "HolyElemental.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class VOID_API AHolyElemental : public AVoidEnemy
{
	GENERATED_BODY()


public:
	AHolyElemental();

	/** Combat Interface */
	virtual void Die() override;

	/** End Combat Interface */

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Dissolve() override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;
	
};
