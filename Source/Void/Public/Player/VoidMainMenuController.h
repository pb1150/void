// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VoidMainMenuController.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class VOID_API AVoidMainMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	AVoidMainMenuController();

protected:
	virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

private:
    UPROPERTY(EditAnywhere, Category = "Camera")
    float MaxRotation = 15.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    TObjectPtr<USoundBase> MainMenuBGM;

    TObjectPtr<UCameraComponent> CachedCamera;
    FRotator TargetRotation;

    void UpdateCameraRotationFromMouse(float DeltaSeconds);


	
};
