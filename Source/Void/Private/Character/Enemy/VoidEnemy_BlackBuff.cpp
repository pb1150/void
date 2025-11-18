// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/VoidEnemy_BlackBuff.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AVoidEnemy_BlackBuff::AVoidEnemy_BlackBuff()
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

void AVoidEnemy_BlackBuff::Die()
{
	Super::Die();
	Dissolve();
}

FVector AVoidEnemy_BlackBuff::GetSpawnActorLocation_Implementation()
{
	if (GetMesh() && SpawnActorSocket.IsValid())
	{
		return GetMesh()->GetSocketLocation(SpawnActorSocket);
	}
	return FVector();
}

void AVoidEnemy_BlackBuff::BeginPlay()
{
	Super::BeginPlay();

	SpawnParticleSystemLocation = GetMesh()->GetBoneLocation(FName("spine_03")) + FVector(0.f, 0.f, 100.f);
}

void AVoidEnemy_BlackBuff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoidEnemy_BlackBuff::Dissolve()
{
	UMaterialInstanceDynamic* DynamicMatInst_0 = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	GetMesh()->SetMaterial(0, DynamicMatInst_0);
	StartDissolveTimeline_0(DynamicMatInst_0);

	UMaterialInstanceDynamic* DynamicMatInst_1 = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), this);
	GetMesh()->SetMaterial(1, DynamicMatInst_1);
	StartDissolveTimeline_1(DynamicMatInst_1);

}
