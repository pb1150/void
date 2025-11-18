// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/VoidEnemy_Buff.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

AVoidEnemy_Buff::AVoidEnemy_Buff()
{

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 450.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 15.f;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

}

void AVoidEnemy_Buff::Die()
{
	Super::Die();
	Dissolve();
}

FVector AVoidEnemy_Buff::GetSpawnActorLocation_Implementation()
{
	if (GetMesh())
	{
		FVector StartPoint = GetMesh()->GetBoneLocation(FName("root")) + FVector(0.f, 0.f, 40.f);
		return (GetActorForwardVector() * 200) + StartPoint;
	}
	return FVector();
}

FVector AVoidEnemy_Buff::GetWarpingLocation_Implementation()
{
	if (CombatTarget == nullptr) return (GetActorLocation() + (GetActorForwardVector() * 200.f));
	FVector Normalize = (GetActorLocation() - CombatTarget->GetActorLocation()).GetSafeNormal();
	return (CombatTarget->GetActorLocation() + (Normalize * BigSmashDistanceToTarget));

}

void AVoidEnemy_Buff::BeginPlay()
{
	Super::BeginPlay();

	SpawnParticleSystemLocation = GetMesh()->GetBoneLocation(FName("root")) + FVector(0.f, 0.f, 50.f);
}

void AVoidEnemy_Buff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoidEnemy_Buff::Dissolve()
{
	UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	GetMesh()->SetMaterial(0, DynamicMatInst);
	StartDissolveTimeline_0(DynamicMatInst);
}
