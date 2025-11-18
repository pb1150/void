// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/ItemInterface.h"

// Add default functionality here for any IItemInterface functions that are not pure virtual.

void IItemInterface::DestroySelf()
{
}

void IItemInterface::HighLightItem(float TriggerTime)
{
}

void IItemInterface::UnHighLightItem()
{
}

FGameplayTag IItemInterface::GetWeaponTag()
{
    return FGameplayTag();
}

bool IItemInterface::IsWeapon()
{
    return false;
}

bool IItemInterface::IsAbilityEffectActor()
{
    return false;
}

void IItemInterface::ApplyAbilitytoActor(AActor* OtherActor)
{
}

FVector IItemInterface::GetBulletSocketLocation()
{
    return FVector();
}

FRotator IItemInterface::GetBulletSocketRotation()
{
    return FRotator();
}

