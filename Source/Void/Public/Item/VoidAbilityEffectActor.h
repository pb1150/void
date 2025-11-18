// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/VoidItem.h"
#include "GameplayTagContainer.h"
#include "VoidAbilityEffectActor.generated.h"

class UVoidAbilityBase;
class UGameplayEffect;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class VOID_API AVoidAbilityEffectActor : public AVoidItem
{
	GENERATED_BODY()

public:
	AVoidAbilityEffectActor();

	/** Item Interface */
	virtual bool IsAbilityEffectActor() override;
	virtual void HighLightItem(float TriggerTime) override;
	virtual void UnHighLightItem() override;
	virtual void ApplyAbilitytoActor(AActor* OtherActor) override;
	/** End Item Interface */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UVoidAbilityBase> ActorAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	void OnOverlay(AActor* OtherActor);

private:
	UPROPERTY(EditAnywhere, Category = "Ability|Sound")
	TObjectPtr<USoundBase> PickupSound;

	UPROPERTY(EditAnywhere, Category = "Ability|Sound")
	bool bPlaySound = false;
	
};
