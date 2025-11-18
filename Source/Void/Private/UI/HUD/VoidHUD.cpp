// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/VoidHUD.h"
#include "UI/Widget/VoidUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/DetailsWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"


UOverlayWidgetController* AVoidHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

UDetailsWidgetController* AVoidHUD::GetDetailsWidgetController(const FWidgetControllerParams& WCParams)
{
	if (DetailsWidgetController == nullptr)
	{
		DetailsWidgetController = NewObject<UDetailsWidgetController>(this, DetailsWidgetControllerClass);
		DetailsWidgetController->SetWidgetControllerParams(WCParams);
		DetailsWidgetController->BindCallbacksToDependencies();
	}
	return DetailsWidgetController;
}

void AVoidHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass uninitialized"));

	UUserWidget* Widget = CreateWidget<UVoidUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UVoidUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* OverlayWC = GetOverlayWidgetController(WidgetControllerParams);


	OverlayWidget->SetWidgetController(OverlayWC);
	OverlayWC->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}

void AVoidHUD::BeginPlay()
{
	Super::BeginPlay();

}
