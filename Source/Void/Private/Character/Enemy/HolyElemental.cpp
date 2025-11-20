// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/HolyElemental.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"

AHolyElemental::AHolyElemental()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 450.f;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AHolyElemental::Die()
{
	Super::Die();
	Dissolve();
}

void AHolyElemental::BeginPlay()
{
	Super::BeginPlay();

	SpawnParticleSystemLocation = GetMesh()->GetBoneLocation(FName("root")) + FVector(0.f, 0.f, 100.f);
}

void AHolyElemental::Destroyed()
{
	UVoidBlueprintFunctionLibrary::SpawnDropItems(this, CharacterClass, GetActorLocation());
	Super::Destroyed();
}

void AHolyElemental::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHolyElemental::Dissolve()
{
	SetLifeSpan(5.f);
}
