// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VoidHUD.generated.h"

class UVoidUserWidget;
class UOverlayWidgetController;
class UDetailsWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;


/**
 * 
 */
UCLASS()
class VOID_API AVoidHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UDetailsWidgetController* GetDetailsWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:
	virtual void BeginPlay() override;



private:

	UPROPERTY()
	TObjectPtr<UVoidUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UVoidUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDetailsWidgetController> DetailsWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UDetailsWidgetController> DetailsWidgetController;

};
