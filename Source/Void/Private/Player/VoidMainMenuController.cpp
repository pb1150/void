// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VoidMainMenuController.h"
#include "Interaction/PlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

AVoidMainMenuController::AVoidMainMenuController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AVoidMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetHideCursorDuringCapture(false); 
	SetInputMode(InputMode);

	IPlayerInterface* ControlledPawn = GetPawn<IPlayerInterface>();
	if (ControlledPawn)
	{
		CachedCamera = ControlledPawn->GetMenuCamera();
	}

	if (MainMenuBGM)
	{
		UGameplayStatics::PlaySound2D(this, MainMenuBGM);
	}

}

void AVoidMainMenuController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCameraRotationFromMouse(DeltaSeconds);
}


void AVoidMainMenuController::UpdateCameraRotationFromMouse(float DeltaSeconds)
{
	if (!CachedCamera) return;

	float MouseX, MouseY;
	if (!GetMousePosition(MouseX, MouseY)) return;

	int32 ViewportX, ViewportY;
	GetViewportSize(ViewportX, ViewportY);

	const float CenterX = ViewportX * 0.5f;
	const float CenterY = ViewportY * 0.5f;

	FVector2D NormalizedOffset((MouseX - CenterX) / CenterX, (MouseY - CenterY) / CenterY);

	TargetRotation.Yaw = FMath::Clamp(NormalizedOffset.X * MaxRotation, -MaxRotation, MaxRotation);
	TargetRotation.Pitch = FMath::Clamp(-NormalizedOffset.Y * MaxRotation, -MaxRotation, MaxRotation);

	FRotator CurrentRot = CachedCamera->GetRelativeRotation();
	FRotator SmoothedRot = FMath::RInterpTo(CurrentRot, TargetRotation, DeltaSeconds, 3.0f);

	CachedCamera->SetRelativeRotation(SmoothedRot);
}
