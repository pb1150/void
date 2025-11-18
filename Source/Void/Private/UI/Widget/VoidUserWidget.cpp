// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/VoidUserWidget.h"

void UVoidUserWidget::SetWidgetController(UObject* NewWidgetController)
{
	WidgetController = NewWidgetController;
	WidgetControllerSet();
}

FText UVoidUserWidget::GetHealthText(float Health, float MaxHealth)
{
	int32 Current = FMath::RoundToInt(Health);
	int32 Max = FMath::RoundToInt(MaxHealth);

	FString HealthString = FString::Printf(TEXT("%d / %d"), Current, Max);
	return FText::FromString(HealthString);
}
