// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VoidMainMenuPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

AVoidMainMenuPawn::AVoidMainMenuPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);

	Title = CreateDefaultSubobject<UWidgetComponent>("Title");
	Title->SetupAttachment(RootComponent);

	ButtonMenu = CreateDefaultSubobject<UWidgetComponent>("ButtonMenu");
	ButtonMenu->SetupAttachment(RootComponent);
}

UCameraComponent* AVoidMainMenuPawn::GetMenuCamera() const
{
	return CameraComponent;
}

void AVoidMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVoidMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVoidMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

