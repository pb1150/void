// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interaction/PlayerInterface.h"
#include "VoidMainMenuPawn.generated.h"

class UCameraComponent;
class UWidgetComponent;

UCLASS()
class VOID_API AVoidMainMenuPawn : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AVoidMainMenuPawn();

	/* Player Interface */
	virtual UCameraComponent* GetMenuCamera() const override;
	/* End Player Interface */

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "MainMenu")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "MainMenu")
	TObjectPtr<UWidgetComponent> Title;

	UPROPERTY(VisibleAnywhere, Category = "MainMenu")
	TObjectPtr<UWidgetComponent> ButtonMenu;
	
};
