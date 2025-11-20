// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnPoint.generated.h"

UCLASS()
class VOID_API AEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnPoint();

protected:

	virtual void BeginPlay() override;


};
