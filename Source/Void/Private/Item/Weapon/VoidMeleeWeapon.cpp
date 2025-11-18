// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Weapon/VoidMeleeWeapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "VoidAbilityTypes.h"
#include "AbilitySystem/VoidBlueprintFunctionLibrary.h"


AVoidMeleeWeapon::AVoidMeleeWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponBox = CreateDefaultSubobject<UBoxComponent>(FName("WeaponBox"));
	WeaponBox->SetupAttachment(WeaponMesh);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Start"));
	BoxTraceStart->SetupAttachment(WeaponBox);
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("End"));
	BoxTraceEnd->SetupAttachment(WeaponBox);
	
}

void AVoidMeleeWeapon::HighLightItem(float TriggerTime)
{
	if (bEquip) return;
	if (M_HighLight && WeaponMesh)
	{
		WeaponMesh->SetOverlayMaterial(M_HighLight);
		PromptBox->SetVisibility(true);
		SetPromptBoxTriggerTime(TriggerTime);
	}
}

void AVoidMeleeWeapon::UnHighLightItem()
{
	if (bEquip) return;
	if (WeaponMesh)
	{
		WeaponMesh->SetOverlayMaterial(nullptr);
		PromptBox->SetVisibility(false);
	}
}

void AVoidMeleeWeapon::bEnableBoxCollision(bool value)
{
	if (value)
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AVoidMeleeWeapon::ResetWeapon()
{
	if (!IgnoreActor.IsEmpty()) IgnoreActor.Empty();
	ShouldShake = true;
}

void AVoidMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponBox)
	{
		WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AVoidMeleeWeapon::OnBoxBeginOverlap);
	}
}

void AVoidMeleeWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == GetOwner()) return;
	if (OtherActor->ActorHasTag("Player")) return;
	if (!OtherActor->ActorHasTag("Enemy")) return;
	if (IgnoreActor.Contains(OtherActor)) return;

	TArray<FHitResult> Hits;
	MultiBoxTrace(Hits);

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!Hit.GetActor()) continue;
		if (HitActor == GetOwner()) continue;
		if (!HitActor->ActorHasTag("Enemy")) continue;
		if (IgnoreActor.Contains(HitActor)) continue;
		IgnoreActor.AddUnique(HitActor);

		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (TargetASC && DamageEffectSpecHandle.IsValid())
		{
			const FGameplayEffectSpec* TemplateSpec = DamageEffectSpecHandle.Data.Get();
			if (TemplateSpec)
			{
				FGameplayEffectSpec NewSpec = DuplicateTemplateSpec(TemplateSpec, Hit);

				TargetASC->ApplyGameplayEffectSpecToSelf(NewSpec);

				FVoidGameplayEffectContext* MyContext = static_cast<FVoidGameplayEffectContext*>(NewSpec.GetContext().Get());
				
				if (MyContext && MyContext->IsHeavyAttack())
				{
					AddHitImpulse(HitActor);
					if (ShouldShake)
					{
						ShouldShake = false;
						AddCameraShake();
					}
					
				}
			}
		}
	}

}

void AVoidMeleeWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector BoxTraceStartLocation = BoxTraceStart->GetComponentLocation();
	const FVector BoxTraceEndLocation = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);
	if (GetOwner()) ActorToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActor) {
		ActorToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(this,
		BoxTraceStartLocation, BoxTraceEndLocation,
		BoxTraceExtent, BoxTraceStart->GetComponentRotation(),
		UEngineTypes::ConvertToTraceType(ECC_Pawn), false,
		ActorToIgnore, bShowBoxTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit, true
	);
	if (BoxHit.bBlockingHit)
	{
		IgnoreActor.AddUnique(BoxHit.GetActor());
	}
}

void AVoidMeleeWeapon::MultiBoxTrace(TArray<FHitResult>& HitResults)
{
	const FVector BoxTraceStartLocation = BoxTraceStart->GetComponentLocation();
	const FVector BoxTraceEndLocation = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);
	if (GetOwner()) ActorToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActor) {
		ActorToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceMulti(
		this,
		BoxTraceStartLocation, BoxTraceEndLocation,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
}


FGameplayEffectSpec AVoidMeleeWeapon::DuplicateTemplateSpec(const FGameplayEffectSpec* TSpec, const FHitResult& SweepResult)
{
	FGameplayEffectContextHandle NewContext = TSpec->GetContext().Duplicate();
	NewContext.AddHitResult(SweepResult);
	UVoidBlueprintFunctionLibrary::SetCanHitReact(NewContext, true);

	FGameplayEffectSpec NewSpec(TSpec->Def, NewContext, TSpec->GetLevel());

	NewSpec.DynamicGrantedTags.AppendTags(TSpec->DynamicGrantedTags);
	NewSpec.CapturedRelevantAttributes = TSpec->CapturedRelevantAttributes;
	NewSpec.DynamicAssetTags.AppendTags(TSpec->DynamicAssetTags);

	for (auto& Pair : TSpec->SetByCallerTagMagnitudes)
	{
		NewSpec.SetSetByCallerMagnitude(Pair.Key, Pair.Value);
	}

	return NewSpec;
}



