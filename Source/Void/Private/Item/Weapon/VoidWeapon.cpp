// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/VoidWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AVoidWeapon::AVoidWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Block);
	CapsuleComponent->SetSimulatePhysics(true);

	PromptBox = CreateDefaultSubobject<UWidgetComponent>("PromptBox");
	PromptBox->SetupAttachment(GetRootComponent());
	PromptBox->SetVisibility(false);
}

void AVoidWeapon::DestroySelf()
{
	Destroy();
}

FGameplayTag AVoidWeapon::GetWeaponTag()
{
	return WeaponTag;
}

bool AVoidWeapon::IsWeapon()
{
	return true;
}

void AVoidWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AVoidWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVoidWeapon::OnEquipWeapon()
{
	bEquip = true;
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PromptBox->Deactivate();
	PromptBox->SetVisibility(false);
}

void AVoidWeapon::OnUnEquipWeapon()
{
	bEquip = false;
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PromptBox->Activate();
	PromptBox->SetVisibility(true);
	SetPromptBoxTriggerTime(0.f);
}

void AVoidWeapon::AddHitImpulse(AActor* OtherActor)
{
	if (ACharacter* HitCharacter = Cast<ACharacter>(OtherActor))
	{
		if (!IsValid(HitCharacter) || HitCharacter->IsActorBeingDestroyed()) return;

		if (HitCharacter->GetCharacterMovement() && HitCharacter->GetMesh())
		{
			UCharacterMovementComponent* MoveComp = HitCharacter->GetCharacterMovement();
			FVector ImpulseDir = (HitCharacter->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
			ImpulseDir.Z += 0.5f;
			ImpulseDir = ImpulseDir.GetSafeNormal();
			FVector Impulse = ImpulseDir * ImpulseStrength;

			if (HitCharacter->GetCharacterMovement() && !HitCharacter->GetMesh()->IsSimulatingPhysics())
			{
				HitCharacter->GetCharacterMovement()->AddImpulse(Impulse, true);
			}
		}
	}
}

void AVoidWeapon::AddCameraShake()
{
	if (APlayerController* PC = GetOwner<ACharacter>()->GetController<APlayerController>())
	{
		if (CameraShakeClass)
		{
			PC->ClientStartCameraShake(CameraShakeClass);
		}
	}
}
