// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TerrainGenerator.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "KismetProceduralMeshLibrary.h"
#include "Async/Async.h"
#include "Game/VoidGameInstance.h"

ATerrainGenerator::ATerrainGenerator()
{
    PrimaryActorTick.bCanEverTick = false;
    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
    SetRootComponent(ProcMesh);
    ProcMesh->bUseAsyncCooking = true;
}

void ATerrainGenerator::BeginPlay()
{
    Super::BeginPlay();
    UVoidGameInstance* VoidGI = Cast<UVoidGameInstance>(UGameplayStatics::GetGameInstance(this));


    Generate();
}

void ATerrainGenerator::Generate()
{
    Vertices.Reset(); 
    Triangles.Reset(); 
    Normals.Reset(); 
    UVs.Reset(); 
    Tangents.Reset();

    GenerateHeightMesh();
    BuildTriangles();

    float BottomZ = -TerrainThickness;
    AddSideWalls(BottomZ);

    ComputeNormals();

    ProcMesh->CreateMeshSection_LinearColor(
        0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
    ProcMesh->ContainsPhysicsTriMeshData(true);
    ProcMesh->SetMobility(EComponentMobility::Static);
    ApplyMaterial();

    SpawnEnemyPoints();

    // 触发导航网格重建（确保有 NavMeshBoundsVolume 覆盖区域）
    if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
    {
        NavSys->Build();
    }


}

float ATerrainGenerator::SampleHeight(int32 X, int32 Y, FRandomStream& Stream) const
{
    FVector2D P(X, Y);

    // 平缓的地形基底
    float base = FMath::PerlinNoise2D(P * 0.0003f) * 0.3f;

    // 初始化高度
    float height = base;

    // 添加月坑特征
    const int32 NumCraters = 25;  // 坑数量，可调
    for (int i = 0; i < NumCraters; ++i)
    {
        Stream.Initialize(Seed + i * 997); // 保证每个坑可复现
        FVector2D craterCenter(Stream.FRandRange(0, GridX), Stream.FRandRange(0, GridY));
        float craterRadius = Stream.FRandRange(8.f, 40.f);   // 坑大小（单位：格）
        float craterDepth = Stream.FRandRange(0.1f, 0.6f);  // 坑深度比例

        // 当前点到坑中心的距离
        float dist = FVector2D::Distance(P, craterCenter);

        // 坑影响范围
        if (dist < craterRadius)
        {
            float normalized = dist / craterRadius; // [0,1]
            // 使用平滑的高斯曲线模拟凹陷
            float craterShape = -craterDepth * FMath::Exp(-normalized * normalized * 4.0f);
            height += craterShape;
        }
    }

    // 4️⃣ 映射到总体高度范围
    return height * HeightAmplitude;
}


void ATerrainGenerator::GenerateHeightMesh()
{
    FRandomStream Stream(Seed);
    const float dx = GetCellSizeX();
    const float dy = GetCellSizeY();

    Vertices.Reserve(GridX * GridY);
    UVs.Reserve(GridX * GridY);

    for (int32 y = 0; y < GridY; ++y)
    {
        for (int32 x = 0; x < GridX; ++x)
        {
            const float Xw = x * dx;
            const float Yw = y * dy;
            const float Z = SampleHeight(x, y, Stream);

            Vertices.Add(FVector(Xw, Yw, Z));
            UVs.Add(FVector2D(float(x) / float(GridX - 1), float(y) / float(GridY - 1)));
        }
    }
}

void ATerrainGenerator::BuildTriangles()
{
    auto IndexAt = [this](int32 x, int32 y) { return y * GridX + x; };
    for (int32 y = 0; y < GridY - 1; ++y)
    {
        for (int32 x = 0; x < GridX - 1; ++x)
        {
            const int32 i0 = IndexAt(x, y);
            const int32 i1 = IndexAt(x + 1, y);
            const int32 i2 = IndexAt(x, y + 1);
            const int32 i3 = IndexAt(x + 1, y + 1);

            // 两个三角形
            Triangles.Add(i0); Triangles.Add(i2); Triangles.Add(i3);
            Triangles.Add(i0); Triangles.Add(i3); Triangles.Add(i1);

        }
    }
}

void ATerrainGenerator::ComputeNormals()
{
    // 创建一个弱指针来追踪当前对象是否还存在
    TWeakObjectPtr<ATerrainGenerator> WeakThis(this);

    // 拷贝局部数据
    TArray<FVector> LocalVertices = Vertices;
    TArray<int32> LocalTriangles = Triangles;
    TArray<FVector2D> LocalUVs = UVs;

    // 后台线程计算
    Async(EAsyncExecution::ThreadPool, [WeakThis, LocalVertices, LocalTriangles, LocalUVs]()
        {
            TArray<FVector> OutNormals;
            TArray<FProcMeshTangent> OutTangents;

            UKismetProceduralMeshLibrary::CalculateTangentsForMesh(LocalVertices, LocalTriangles, LocalUVs, OutNormals, OutTangents);

            // 回主线程更新
            AsyncTask(ENamedThreads::GameThread, [WeakThis, OutNormals, OutTangents]()
                {
                    // 检查对象是否还存在
                    if (!WeakThis.IsValid())
                    {
                        UE_LOG(LogTemp, Warning, TEXT("TerrainGenerator destroyed before normals update finished."));
                        return;
                    }

                    ATerrainGenerator* Generator = WeakThis.Get();
                    if (!IsValid(Generator->ProcMesh))
                    {
                        UE_LOG(LogTemp, Warning, TEXT("ProcMesh invalid when updating normals."));
                        return;
                    }

                    Generator->Normals = OutNormals;
                    Generator->Tangents = OutTangents;

                    Generator->ProcMesh->UpdateMeshSection_LinearColor(
                        0,
                        Generator->Vertices,
                        Generator->Normals,
                        Generator->UVs,
                        TArray<FLinearColor>(),
                        Generator->Tangents
                    );

                    UE_LOG(LogTemp, Log, TEXT("Terrain normals updated safely (async)."));
                });
        });
}



void ATerrainGenerator::ApplyMaterial()
{
    if (TerrainMaterial)
    {
        ProcMesh->SetMaterial(0, TerrainMaterial);
        // 在材质里用 WorldAlignedBlend 或 基于 Z/法线 进行自动混合
    }
}

float ATerrainGenerator::LocalSlopeDeg(int32 X, int32 Y) const
{
    auto Idx = [this](int32 x, int32 y) { return y * GridX + x; };
    const int32 ix = FMath::Clamp(X, 1, GridX - 2);
    const int32 iy = FMath::Clamp(Y, 1, GridY - 2);

    const FVector& P = Vertices[Idx(ix, iy)];
    const FVector& Px = Vertices[Idx(ix + 1, iy)];
    const FVector& Py = Vertices[Idx(ix, iy + 1)];
    const FVector Nx = (Px - P).GetSafeNormal();
    const FVector Ny = (Py - P).GetSafeNormal();
    const FVector Up = FVector::UpVector;

    // 用相邻两方向近似法线
    const FVector N = FVector::CrossProduct(Nx, Ny).GetSafeNormal();
    const float Cos = FVector::DotProduct(N, Up);
    const float Deg = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(Cos, -1.f, 1.f)));
    return Deg;
}

void ATerrainGenerator::SpawnEnemyPoints()
{
    if (!EnemySpawnerClass) return;

    FRandomStream Stream(Seed * 33 + 7);
    for (int32 i = 0; i < SpawnSampleCount; ++i)
    {
        const int32 rx = Stream.RandRange(1, GridX - 2);
        const int32 ry = Stream.RandRange(1, GridY - 2);
        const int32 idx = ry * GridX + rx;

        const float SlopeDeg = LocalSlopeDeg(rx, ry);
        if (SlopeDeg > MaxAllowedSlopeDeg) continue;

        const FVector Loc = GetActorTransform().TransformPosition(Vertices[idx] + FVector(0, 0, 10));
        GetWorld()->SpawnActor<AActor>(EnemySpawnerClass, Loc, FRotator::ZeroRotator);
    }
}

void ATerrainGenerator::AddSideWalls(float BottomZ)
{
    // 创建底面顶点（和顶层一一对应）
    int32 TopVertexCount = Vertices.Num();
    for (int32 i = 0; i < TopVertexCount; ++i)
    {
        FVector v = Vertices[i];
        v.Z = BottomZ;
        Vertices.Add(v);
    }

    auto IdxTop = [&](int32 x, int32 y) { return y * GridX + x; };
    auto IdxBottom = [&](int32 x, int32 y) { return IdxTop(x, y) + TopVertexCount; };

    // 四个侧面
    // 前边（Y=0）
    for (int32 x = 0; x < GridX - 1; ++x)
    {
        int32 t0 = IdxTop(x, 0);
        int32 t1 = IdxTop(x + 1, 0);
        int32 b0 = IdxBottom(x, 0);
        int32 b1 = IdxBottom(x + 1, 0);

        Triangles.Add(t0); Triangles.Add(b1); Triangles.Add(b0);
        Triangles.Add(t0); Triangles.Add(t1); Triangles.Add(b1);
    }

    // 后边（Y=GridY-1）
    for (int32 x = 0; x < GridX - 1; ++x)
    {
        int32 t0 = IdxTop(x, GridY - 1);
        int32 t1 = IdxTop(x + 1, GridY - 1);
        int32 b0 = IdxBottom(x, GridY - 1);
        int32 b1 = IdxBottom(x + 1, GridY - 1);

        Triangles.Add(t0); Triangles.Add(b0); Triangles.Add(b1);
        Triangles.Add(t0); Triangles.Add(b1); Triangles.Add(t1);
    }

    // 左边（X=0）
    for (int32 y = 0; y < GridY - 1; ++y)
    {
        int32 t0 = IdxTop(0, y);
        int32 t1 = IdxTop(0, y + 1);
        int32 b0 = IdxBottom(0, y);
        int32 b1 = IdxBottom(0, y + 1);

        Triangles.Add(t0); Triangles.Add(b0); Triangles.Add(b1);
        Triangles.Add(t0); Triangles.Add(b1); Triangles.Add(t1);
    }

    // 右边（X=GridX-1）
    for (int32 y = 0; y < GridY - 1; ++y)
    {
        int32 t0 = IdxTop(GridX - 1, y);
        int32 t1 = IdxTop(GridX - 1, y + 1);
        int32 b0 = IdxBottom(GridX - 1, y);
        int32 b1 = IdxBottom(GridX - 1, y + 1);

        Triangles.Add(t0); Triangles.Add(b1); Triangles.Add(b0);
        Triangles.Add(t0); Triangles.Add(t1); Triangles.Add(b1);
    }

    // 底面封底（顺序反向，让法线朝下）
    for (int32 y = 0; y < GridY - 1; ++y)
    {
        for (int32 x = 0; x < GridX - 1; ++x)
        {
            int32 i0 = IdxBottom(x, y);
            int32 i1 = IdxBottom(x + 1, y);
            int32 i2 = IdxBottom(x, y + 1);
            int32 i3 = IdxBottom(x + 1, y + 1);

            // 注意这里顺序反转，让法线朝下
            Triangles.Add(i0); Triangles.Add(i3); Triangles.Add(i2);
            Triangles.Add(i0); Triangles.Add(i1); Triangles.Add(i3);

        }
    }
}



