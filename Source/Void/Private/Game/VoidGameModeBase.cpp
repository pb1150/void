// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VoidGameModeBase.h"
#include "Game/VoidGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Item/EnemySpawnPoint.h"
#include "Character/VoidEnemy.h"
#include "Interaction/CombatInterface.h"
#include "GameFramework/PlayerStart.h"


AActor* AVoidGameModeBase::ChoosePlayerStart_Implementation(AController* LocalPlayer)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerStart found, using default."));
		return Super::ChoosePlayerStart_Implementation(LocalPlayer);
	}

	const int32 Index = FMath::RandRange(0, PlayerStarts.Num() - 1);
	return PlayerStarts[Index];
}

void AVoidGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentAttributeEffectItemNum = 0;
	CurrentAbilityEffectItemNum = 0;
	KilledEnemyCount = 0;
	

	if (VoidGameInstance = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		VoidGameInstance->HideLoadingScreen();
		MaxKilledEnemyCount = VoidGameInstance->GetThisLevelMaxKilledEnemyCount();

		if (VoidGameInstance->CurrentLevelIndex != 0)
		{
			
			UGameplayStatics::GetAllActorsWithTag(this, FName("Player"), Players);
			if (!Players.IsEmpty())
			{
				Player = Players[0];
				if (Player->Implements<UCombatInterface>())
				{
					PlayerInterface = Cast<ICombatInterface>(Player);
				}
			}

			UGameplayStatics::GetAllActorsWithTag(this, FName("Transmitter"), Transmitters);
			if (!Transmitters.IsEmpty())
			{
				Transmitter = Transmitters[0];
			}

			TArray<AActor*> TempActors;
			UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawnPoint::StaticClass(), TempActors);
			AllSpawnPoints.Empty();
			for (AActor* Actor : TempActors)
			{
				if (AEnemySpawnPoint* SpawnPoint = Cast<AEnemySpawnPoint>(Actor))
				{
					AllSpawnPoints.Add(SpawnPoint);
				}
			}

			GetWorldTimerManager().SetTimer(EnemySpawnTimerHandle, this, &AVoidGameModeBase::TrySpawnEnemies, SpawnInterval, true);
		}
		
	}

}

void AVoidGameModeBase::TrySpawnEnemies()
{
	if (!CharacterClassInfo) return;


	if (KilledEnemyCount >= MaxKilledEnemyCount)
	{
		if (!Transmitter || !TransmitterClass) return;
		GetWorldTimerManager().ClearTimer(EnemySpawnTimerHandle);
		FVector SpawnLocation = Transmitter->GetActorLocation();
		FRotator SpawnRotation = Transmitter->GetActorRotation();
		GetWorld()->SpawnActor<AActor>(TransmitterClass, SpawnLocation, SpawnRotation);
		return;
	}

	int32 Missing = MaxValidEnemyCount - CurrentEnemyCount;
	if (Missing <= 0) return;

	int32 EnemyLevel = 1;
	if (PlayerInterface && !ICombatInterface::Execute_IsDead(Player))
	{
		EnemyLevel = PlayerInterface->GetPlayerLevel();
	}
	else
	{
		return;
	}

	if (VoidGameInstance)
	{
		EnemyLevel += VoidGameInstance->GetTotalRuntimeToInt();
	}
	
	for (int32 i = 0; i < Missing; ++i)
	{
		AEnemySpawnPoint* SpawnPoint = GetRandomAvailableSpawnPoint();
		if (!SpawnPoint) break;

		TSubclassOf<AVoidEnemy> EnemyClass = GetRandomEnemyClassFromData();
		if (!EnemyClass) continue;

		const float RandomYaw = FMath::FRandRange(0.f, 360.f);
		const FVector SpawnLoc = SpawnPoint->GetActorLocation() + FVector(0.f, 0.f, 30.f);
		const FRotator SpawnRot = FRotator(0.f, RandomYaw, 0.f);
		const FTransform SpawnTransform(SpawnRot, SpawnLoc);

		AVoidEnemy* Enemy = GetWorld()->SpawnActorDeferred<AVoidEnemy>(EnemyClass, SpawnTransform);
		if (Enemy)
		{
			Enemy->Level = EnemyLevel;
			Enemy->OnDestroyed.AddDynamic(this, &AVoidGameModeBase::OnEnemyKilled);

			UGameplayStatics::FinishSpawningActor(Enemy, SpawnTransform);
			++CurrentEnemyCount;
		}
	}

	AvailableSpawnPoints.Empty();
}

AEnemySpawnPoint* AVoidGameModeBase::GetRandomAvailableSpawnPoint()
{
	if (!Player) return nullptr;
	const FVector PlayerLoc = Player->GetActorLocation();

	if (AvailableSpawnPoints.IsEmpty())
	{
		for (AEnemySpawnPoint* Point : AllSpawnPoints)
		{
			if (!Point) continue;

			const float Distance = FVector::Dist(Point->GetActorLocation(), PlayerLoc);
			if (Distance <= ValidSpawnRadius)
			{
				AvailableSpawnPoints.Add(Point);
			}
		}

		if (AvailableSpawnPoints.Num() == 0)
		{
			for (AEnemySpawnPoint* Point : AllSpawnPoints)
			{
				if (Point)
				{
					AvailableSpawnPoints.Add(Point);
				}
			}
		}
	}

	if (AvailableSpawnPoints.Num() == 0)
	{
		return nullptr;
	}

	int32 Index = FMath::RandRange(0, AvailableSpawnPoints.Num() - 1);
	AEnemySpawnPoint* Chosen = AvailableSpawnPoints[Index];
	AvailableSpawnPoints.RemoveAt(Index);
	return Chosen;
}

TSubclassOf<AVoidEnemy> AVoidGameModeBase::GetRandomEnemyClassFromData()
{
	if (!CharacterClassInfo) return nullptr;

	TArray<ECharacterClass> Keys;
	CharacterClassInfo->CharacterClassInfomation.GetKeys(Keys);

	float TotalWeight = 0.f;
	for (ECharacterClass Key : Keys)
	{
		TotalWeight += CharacterClassInfo->CharacterClassInfomation[Key].SpawnProbability;
	}

	float Rand = FMath::FRandRange(0.f, TotalWeight);
	float Accum = 0.f;

	for (ECharacterClass Key : Keys)
	{
		const FCharacterClassDefaultInfo& Info = CharacterClassInfo->CharacterClassInfomation[Key];
		Accum += Info.SpawnProbability;
		if (Rand <= Accum)
		{
			return Info.EnemyClass;
		}
	}

	return nullptr;
}

void AVoidGameModeBase::OnEnemyKilled(AActor* EnemyActor)
{
	CurrentEnemyCount = FMath::Max(0, CurrentEnemyCount - 1);
	KilledEnemyCount++;
}
