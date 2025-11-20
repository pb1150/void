// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VoidGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

UVoidGameInstance::UVoidGameInstance()
{
	
}

int32 UVoidGameInstance::GetTotalRuntimeToInt() const
{
	if (GameStartTime <= 0.f)
	{
		return 0;
	}

	const double Now = FPlatformTime::Seconds();
	const double Delta = Now - GameStartTime;
	UE_LOG(LogTemp, Warning, TEXT("NowSeconds:%f"), Now);
	UE_LOG(LogTemp, Warning, TEXT("GameStartTime:%f"), GameStartTime);

	const int32 Minutes = FMath::FloorToInt((float)Delta / 60.f);
	return FMath::Max(0, Minutes);
}

void UVoidGameInstance::Init()
{
	Super::Init();
	CurrentLevelIndex = 0;
	LastReachedLevel = 0;
}

void UVoidGameInstance::OnVoidLevelLoaded()
{
	const FLevelDefaultInfo& LevelInfo = LevelDataAsset->LevelDefaultList[CurrentLevelIndex];
	LastReachedLevel++;
	if (CurrentLevelIndex == 0)
	{
		LastReachedLevel = 0;
	}
	if (UWorld* LoadedWorld = LevelInfo.LevelRef.Get())
	{
		FString LevelName = LoadedWorld->GetName();
		UGameplayStatics::OpenLevel(this, FName(*LevelName));
	}
}

int32 UVoidGameInstance::GetThisLevelMaxKilledEnemyCount()
{
	const FLevelDefaultInfo& LevelInfo = LevelDataAsset->LevelDefaultList[CurrentLevelIndex];
	return LevelInfo.MaxKilledEnemyCount;
}

