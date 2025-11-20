// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem\Data\LevelInfo.h"
#include "ProceduralMeshComponent.h"
#include "TerrainGenerator.generated.h"

UCLASS()
class ATerrainGenerator : public AActor
{
    GENERATED_BODY()
public:
    ATerrainGenerator();

    UPROPERTY(EditAnywhere, Category = "Terrain|Size")
    int32 GridX = 256;

    UPROPERTY(EditAnywhere, Category = "Terrain|Size")
    int32 GridY = 256;

    UPROPERTY(EditAnywhere, Category = "Terrain|Size")
    float MapSizeX = 10000.f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Size")
    float MapSizeY = 10000.f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Noise")
    float NoiseScale = 0.0015f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Noise")
    float HeightAmplitude = 800.f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Seed")
    int32 Seed = 12345;

    UPROPERTY(EditAnywhere, Category = "Terrain|Spawn")
    TSubclassOf<AActor> EnemySpawnerClass;

    UPROPERTY(EditAnywhere, Category = "Terrain|Spawn")
    int32 SpawnSampleCount = 200;

    UPROPERTY(EditAnywhere, Category = "Terrain|Spawn")
    float MaxAllowedSlopeDeg = 25.f;

    UPROPERTY(EditAnywhere, Category = "Terrain|Mesh")
    float TerrainThickness = 1000.f; 

    UPROPERTY(EditAnywhere, Category = "Terrain|Material")
    UMaterialInterface* TerrainMaterial;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UProceduralMeshComponent* ProcMesh;


    TArray<FVector> Vertices;
    TArray<int32>  Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    void Generate();
    void GenerateHeightMesh();
    void BuildTriangles();
    void ComputeNormals();
    void ApplyMaterial();
    void SpawnEnemyPoints();
    void AddSideWalls(float BottomZ);

    float GetCellSizeX() const { return MapSizeX / float(FMath::Max(1, GridX - 1)); }
    float GetCellSizeY() const { return MapSizeY / float(FMath::Max(1, GridY - 1)); }
    float SampleHeight(int32 X, int32 Y, FRandomStream& Stream) const;
    float LocalSlopeDeg(int32 X, int32 Y) const;
};
