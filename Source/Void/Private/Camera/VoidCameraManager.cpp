// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/VoidCameraManager.h"
#include "GameFramework/Character.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Particles/EmitterCameraLensEffectBase.h"


void AVoidCameraManager::PlayEffect(TSubclassOf<AEmitterCameraLensEffectBase> LensEffectClass)
{
    if (!LensEffectClass) return;
    Effect = AddCameraLensEffect(LensEffectClass);

}

void AVoidCameraManager::CancelEffect()
{
    if (!Effect) return;
    RemoveCameraLensEffect(Effect);
    Effect->Destroy();
    Effect = nullptr;
}

void AVoidCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{

    Super::UpdateViewTarget(OutVT, DeltaTime);

    if (!PCOwner || !PCOwner->GetPawn()) return;

    ACharacter* Character = PCOwner->GetPawn<ACharacter>();
    if (!Character) return;


    // 限制 Pitch
    OutVT.POV.Rotation.Pitch = FMath::ClampAngle(OutVT.POV.Rotation.Pitch, MinPitch, MaxPitch);

    const FVector TargetLoc = OutVT.POV.Location;

    // 初始化一次平滑位置，避免开局跳变
    if (SmoothedLoc.IsNearlyZero())
    {
        SmoothedLoc = TargetLoc;
    }

    // 计算垂直速度
    static float LastZ = 0.f;
    const float CurrentZ = Character->GetActorLocation().Z;
    const float ZSpeed = DeltaTime > 0.f ? (CurrentZ - LastZ) / DeltaTime : 0.f;
    LastZ = CurrentZ;

    const float AbsZSpeed = FMath::Abs(ZSpeed);

    // 映射速度 → 平滑插值系数
    const float SpeedScale = FMath::GetMappedRangeValueClamped(
        FVector2D(0.f, 2000.f),
        FVector2D(1.0f, 40.f),
        AbsZSpeed
    );

    const float FinalInterpSpeed = InterpSpeed * SpeedScale;

    // Z 轴平滑，XY 实时跟随
    SmoothedLoc.Z = FMath::FInterpTo(SmoothedLoc.Z, TargetLoc.Z, DeltaTime, FinalInterpSpeed);
    SmoothedLoc.X = TargetLoc.X;
    SmoothedLoc.Y = TargetLoc.Y;

    // 最终把修改写回 POV
    OutVT.POV.Location = SmoothedLoc;
}



