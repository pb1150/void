// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/VoidWidgetController.h"
#include "DetailsWidgetController.generated.h"

class UAttributeInfo;
class UAttributeEffectActorsInfo;
struct FVoidAttributeInfo;
struct FAttributeActorInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FVoidAttributeInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeEffectActorInfoSignature, const FAttributeActorInfo&, ActorInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VOID_API UDetailsWidgetController : public UVoidWidgetController
{
	GENERATED_BODY()

public:
	UDetailsWidgetController();

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute Actors")
	FAttributeEffectActorInfoSignature AttributeEffectActorInfoDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeEffectActorsInfo> AttributeEffectActorInfo;
	
};
