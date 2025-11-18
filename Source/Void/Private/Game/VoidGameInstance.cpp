// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VoidGameInstance.h"
#include "Kismet/GameplayStatics.h"

UVoidGameInstance::UVoidGameInstance()
{
	
}

int32 UVoidGameInstance::GetTotalRuntimeToInt() const
{
	UWorld* World = GetWorld();
	if (!World) return 0;

	float Delta = UGameplayStatics::GetRealTimeSeconds(World) - GameStartTime;

	return static_cast<int32>(Delta / 60.f); 
}


void UVoidGameInstance::Init()
{
	Super::Init();
	CurrentLevelIndex = 0;
	GameStartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

void UVoidGameInstance::OnVoidLevelLoaded()
{
	const FLevelDefaultInfo& LevelInfo = LevelDataAsset->LevelDefaultList[CurrentLevelIndex];
	if (UWorld* LoadedWorld = LevelInfo.LevelRef.Get())
	{
		CurrentLevelType = LevelDataAsset->LevelDefaultList[CurrentLevelIndex].LevelType;
		FString LevelName = LoadedWorld->GetName();
		UGameplayStatics::OpenLevel(this, FName(*LevelName));
	}
}

int32 UVoidGameInstance::GetThisLevelMaxKilledEnemyCount()
{
	const FLevelDefaultInfo& LevelInfo = LevelDataAsset->LevelDefaultList[CurrentLevelIndex];
	return LevelInfo.MaxKilledEnemyCount;
}

