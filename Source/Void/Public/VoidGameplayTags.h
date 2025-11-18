// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */


struct FVoidGameplayTags
{
public:
	static const FVoidGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attribute_Primary_Level;
	FGameplayTag Attribute_Primary_Damage;
	FGameplayTag Attribute_Primary_MaxHealth;

	FGameplayTag Attribute_Secondary_Health;
	FGameplayTag Attribute_Secondary_AddedMaxHealth;
	FGameplayTag Attribute_Secondary_MoveSpeedMultiplier;
	FGameplayTag Attribute_Secondary_SprintSpeedMultiplier;
	FGameplayTag Attribute_Secondary_HealthRegenRate;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitMultiplier;
	FGameplayTag Attribute_Secondary_DamageReduction;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_AttackSpeedMultiplier;
	FGameplayTag Attribute_Secondary_MaxJumpCount;

	FGameplayTag Attribute_Meta_IncomingXP;

	FGameplayTag GameplayCue_ThrowDagger;
	FGameplayTag GameplayCue_BaseballBat;
	FGameplayTag GameplayCue_BlueSword;
	FGameplayTag GameplayCue_Gun;
	FGameplayTag GameplayCue_TheWorld;

	FGameplayTag Message_MaxHealthPotion;
	FGameplayTag Message_PulseDriveBooster; //加跑步速度的
	FGameplayTag Message_StrideOptimizer;   //加移动速度的
	FGameplayTag Message_RepulsionPlate;	//推斥护甲板
	FGameplayTag Message_CombatStimulants;	//士兵的注射器
	FGameplayTag Message_HealthRegenBooster;//健康再生增强剂
	FGameplayTag Message_CriticalEyeglasses;//暴击眼镜
	FGameplayTag Message_CriticalDamageGear;//暴击伤害齿轮
	FGameplayTag Message_MoreDifficultTimes;//更艰难的时光
	FGameplayTag Message_Test;
	FGameplayTag Message_Ability_Knife;

	FGameplayTag Input_Skill01;
	FGameplayTag Input_Skill02;
	FGameplayTag Input_Skill03;
	FGameplayTag Input_Skill04;
	FGameplayTag Input_Skill05;

	FGameplayTag Ability_Slot01;
	FGameplayTag Ability_Slot02;
	FGameplayTag Ability_BaseBall;
	FGameplayTag Ability_BlueSword;
	FGameplayTag Ability_Rifle;
	FGameplayTag Ability_ThrowDagger;
	FGameplayTag Ability_TheWorld;
	FGameplayTag Ability_ShootCable;
	FGameplayTag Ability_Lightning;
	FGameplayTag Ability_FireTornado;
	FGameplayTag Ability_HealArea;
	FGameplayTag Ability_BlackHole;

	FGameplayTag Cooldown_Ability_ThrowDagger;
	FGameplayTag Cooldown_Ability_TheWorld;
	FGameplayTag Cooldown_Ability_ShootCable;
	FGameplayTag Cooldown_Ability_Lightning;
	FGameplayTag Cooldown_Ability_FireTornado;
	FGameplayTag Cooldown_Ability_HealArea;
	FGameplayTag Cooldown_Ability_BlackHole;

	FGameplayTag Ability_Enemy_Spawn;
	FGameplayTag Ability_Enemy_Melee;
	FGameplayTag Ability_Enemy_RedBuff_Uppercut;
	FGameplayTag Ability_Enemy_RedBuff_BigSmash;
	FGameplayTag Ability_Enemy_BlackBuff_SpitFireBall;

	FGameplayTag Event_Enemy_RightMelee;
	FGameplayTag Event_Enemy_LeftMelee;
	FGameplayTag Event_Enemy_ComboMelee;

	FGameplayTag Weapon_Melee_AttackHeavy;
	FGameplayTag Weapon_Melee_AttackLight;
	FGameplayTag Weapon_Melee_BaseballBat;
	FGameplayTag Weapon_Melee_BlueSword;
	FGameplayTag Weapon_Range_Gun;
	FGameplayTag Weapon_None;

	FGameplayTag Data_Enemy_Level;

	FGameplayTag Damage;

	FGameplayTag Effect_HitReact;

	TMap<FGameplayTag, FGameplayTag> WeaponTagtoCue;


protected:



private:
	static FVoidGameplayTags GameplayTags;


};