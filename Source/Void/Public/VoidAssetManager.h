// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "VoidAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API UVoidAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:


	static UVoidAssetManager& Get();


protected:

	virtual void StartInitialLoading() override;

	
};
