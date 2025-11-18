// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/VoidAbilityEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/VoidAbilitySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

AVoidAbilityEffectActor::AVoidAbilityEffectActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetGenerateOverlapEvents(false);

	PromptBox = CreateDefaultSubobject<UWidgetComponent>("PromptBox");
	PromptBox->SetupAttachment(Mesh);
	PromptBox->SetVisibility(false);
}

void AVoidAbilityEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AVoidAbilityEffectActor::IsAbilityEffectActor()
{
	return true;
}

void AVoidAbilityEffectActor::HighLightItem(float TriggerTime)
{
	if (M_HighLight && Mesh)
	{
		Mesh->SetOverlayMaterial(M_HighLight);
		PromptBox->SetVisibility(true);
		SetPromptBoxTriggerTime(TriggerTime);
	}
}

void AVoidAbilityEffectActor::UnHighLightItem()
{
	if (Mesh)
	{
		Mesh->SetOverlayMaterial(nullptr);
		PromptBox->SetVisibility(false);
	}
}

void AVoidAbilityEffectActor::ApplyAbilitytoActor(AActor* OtherActor)
{
	OnOverlay(OtherActor);
}

void AVoidAbilityEffectActor::OnOverlay(AActor* OtherActor)
{
	if (!OtherActor->ActorHasTag("Player")) return;
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	UVoidAbilitySystemComponent* VoidASC = Cast<UVoidAbilitySystemComponent>(TargetASC);
	if (VoidASC)
	{
		if (IsValid(GameplayEffectClass))
		{
			FGameplayEffectContextHandle ContextHandle = VoidASC->MakeEffectContext();
			FGameplayEffectSpecHandle SpecHandle = VoidASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, ContextHandle);
			VoidASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		VoidASC->ChangeAbility(ActorAbility);
		if (bPlaySound)
		{
			UGameplayStatics::PlaySound2D(this, PickupSound);
		}

		Destroy();
	}

}
