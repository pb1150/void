// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Weapon/VoidWeapon.h"
#include "VoidMeleeWeapon.generated.h"

class UBoxComponent;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class VOID_API AVoidMeleeWeapon : public AVoidWeapon
{
	GENERATED_BODY()

public:
	AVoidMeleeWeapon();

	/** Item Interface */
	virtual void HighLightItem(float TriggerTime) override;
	virtual void UnHighLightItem() override;
	/** End Item Interface */

	TArray<AActor*> IgnoreActor;

	UFUNCTION(BlueprintCallable)
	void bEnableBoxCollision(bool value);

	UFUNCTION(BlueprintCallable)
	void ResetWeapon();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<UBoxComponent> WeaponBox;


private:

	bool ShouldShake = true;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditDefaultsOnly)
	FVector BoxTraceExtent = FVector(2.5f);

	UPROPERTY(EditDefaultsOnly)
	bool bShowBoxTrace = false;
	
	void BoxTrace(FHitResult& BoxHit);
	void MultiBoxTrace(TArray<FHitResult>& HitResults);
	FGameplayEffectSpec DuplicateTemplateSpec(const FGameplayEffectSpec* TSpec, const FHitResult& SweepResult);

};
