// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/VoidEnemyMeleeAttack.h"
#include "Interaction/EnemyInterface.h"

float UVoidEnemyMeleeAttack::GetMeleeAttackRadius()
{
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->Implements<UEnemyInterface>()) return 50.f;

	ECharacterClass CharacterClass = IEnemyInterface::Execute_GetCharacterClass(GetAvatarActorFromActorInfo());
	float* Radius = CharcaterClassToMeleeAttackRadius.Find(CharacterClass);

	float FinalRadius = (Radius != nullptr) ? *Radius : 50.f;
	return FinalRadius;
}
