// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/ItemInterface.h"
#include "VoidItem.generated.h"

class UWidgetComponent;

UCLASS()
class VOID_API AVoidItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	

	AVoidItem();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	TObjectPtr<UMaterial> M_HighLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> PromptBox;

	UFUNCTION(BlueprintImplementableEvent)
	void SetPromptBoxTriggerTime(float NewTime);

private:



};
