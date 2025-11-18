// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "VoidCameraManager.generated.h"

class AEmitterCameraLensEffectBase;

/**
 * 
 */
UCLASS()
class VOID_API AVoidCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void PlayEffect(TSubclassOf<AEmitterCameraLensEffectBase> LensEffectClass);
 
	UFUNCTION(BlueprintCallable)
	void CancelEffect();

	AEmitterCameraLensEffectBase* Effect;


protected:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interp")
	float InterpSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float MinPitch = -75.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float MaxPitch = 90.f;

private:
	FVector SmoothedLoc;
	
};
