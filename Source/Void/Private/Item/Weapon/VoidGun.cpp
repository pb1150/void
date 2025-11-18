// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/VoidGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"

AVoidGun::AVoidGun()
{
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>("GunMesh");
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GunMesh->SetupAttachment(GetRootComponent());

}

void AVoidGun::BeginPlay()
{
	Super::BeginPlay();
}

void AVoidGun::HighLightItem(float TriggerTime)
{
	if (bEquip) return;
	if (M_HighLight && GunMesh)
	{
		GunMesh->SetOverlayMaterial(M_HighLight);
		PromptBox->SetVisibility(true);
		SetPromptBoxTriggerTime(TriggerTime);
	}
}

void AVoidGun::UnHighLightItem()
{
	if (bEquip) return;
	if (GunMesh)
	{
		GunMesh->SetOverlayMaterial(nullptr);
		PromptBox->SetVisibility(false);
	}
}

FVector AVoidGun::GetBulletSocketLocation()
{
	if (GunMesh)
	{
		return GunMesh->GetSocketLocation(BulletSocket_0);
	}
	return FVector();
}

FRotator AVoidGun::GetBulletSocketRotation()
{
	if (GunMesh)
	{
		return (GunMesh->GetSocketLocation(BulletSocket_0) - GunMesh->GetSocketLocation(BulletSocket_1)).Rotation();
	}

	return FRotator();
}


