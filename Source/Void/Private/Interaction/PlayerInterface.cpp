// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/PlayerInterface.h"

// Add default functionality here for any IPlayerInterface functions that are not pure virtual.

bool IPlayerInterface::PlayerSprint()
{
	return false;
}

void IPlayerInterface::EquipWeaponByTag(const FGameplayTag& Tag)
{

}

UCameraComponent* IPlayerInterface::GetMenuCamera() const
{
	return nullptr;
}


