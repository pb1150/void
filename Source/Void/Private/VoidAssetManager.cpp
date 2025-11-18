// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidAssetManager.h"
#include "VoidGameplayTags.h"

UVoidAssetManager& UVoidAssetManager::Get()
{
	check(GEngine);
	UVoidAssetManager* VoidAssetManager = Cast<UVoidAssetManager>(GEngine->AssetManager);
	check(VoidAssetManager);
	return *VoidAssetManager;
}

void UVoidAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FVoidGameplayTags::Get().InitializeNativeGameplayTags();
}
