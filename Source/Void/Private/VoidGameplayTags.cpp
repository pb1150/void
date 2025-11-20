// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidGameplayTags.h"
#include "GameplayTagsManager.h"


FVoidGameplayTags FVoidGameplayTags::GameplayTags;

void FVoidGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attribute_Primary_Level = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Level"));
	GameplayTags.Attribute_Primary_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Damage"));
	GameplayTags.Attribute_Primary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.MaxHealth"));

	GameplayTags.Attribute_Secondary_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Health"));
	GameplayTags.Attribute_Secondary_AddedMaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.AddedMaxHealth"));
	GameplayTags.Attribute_Secondary_MoveSpeedMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MoveSpeedMultiplier"));
	GameplayTags.Attribute_Secondary_SprintSpeedMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.SprintSpeedMultiplier"));
	GameplayTags.Attribute_Secondary_HealthRegenRate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.HealthRegenRate"));
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"));
	GameplayTags.Attribute_Secondary_CriticalHitMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitMultiplier"));
	GameplayTags.Attribute_Secondary_DamageReduction = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.DamageReduction"));
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.BlockChance"));
	GameplayTags.Attribute_Secondary_AttackSpeedMultiplier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.AttackSpeedMultiplier"));
	GameplayTags.Attribute_Secondary_MaxJumpCount = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxJumpCount"));

	GameplayTags.Attribute_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Meta.IncomingXP"));

	GameplayTags.GameplayCue_BaseballBat = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.BaseballBat"));
	GameplayTags.GameplayCue_Gun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Gun"));
	GameplayTags.GameplayCue_BlueSword = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.BlueSword"));
	GameplayTags.GameplayCue_ThrowDagger = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.ThrowDagger"));
	GameplayTags.GameplayCue_TheWorld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.TheWorld"));

	GameplayTags.Message_MaxHealthPotion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.MaxHealthPotion"));
	GameplayTags.Message_PulseDriveBooster = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.PulseDriveBooster"));
	GameplayTags.Message_StrideOptimizer = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.StrideOptimizer"));
	GameplayTags.Message_CombatStimulants = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.CombatStimulants"));
	GameplayTags.Message_CriticalDamageGear = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.CriticalDamageGear"));
	GameplayTags.Message_CriticalEyeglasses = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.CriticalEyeglasses"));
	GameplayTags.Message_HealthRegenBooster = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.HealthRegenBooster"));
	GameplayTags.Message_MoreDifficultTimes = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.MoreDifficultTimes"));
	GameplayTags.Message_RepulsionPlate = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.RepulsionPlate"));
	GameplayTags.Message_Test = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Test"));
	GameplayTags.Message_Ability_Knife = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Message.Ability.Knife"));

	GameplayTags.Input_Skill01 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Skill01"));
	GameplayTags.Input_Skill02 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Skill02"));
	GameplayTags.Input_Skill03 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Skill03"));
	GameplayTags.Input_Skill04 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Skill04"));
	GameplayTags.Input_Skill05 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Skill05"));

	GameplayTags.Ability_Slot01 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Slot01"));
	GameplayTags.Ability_Slot02 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Slot02"));
	GameplayTags.Ability_BaseBall = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.BaseBall"));
	GameplayTags.Ability_BlueSword = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.BlueSword"));
	GameplayTags.Ability_Rifle = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Rifle"));
	GameplayTags.Ability_ThrowDagger = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.ThrowDagger")); 
	GameplayTags.Ability_TheWorld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.TheWorld"));
	GameplayTags.Ability_ShootCable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.ShootCable"));
	GameplayTags.Ability_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Lightning"));
	GameplayTags.Ability_FireTornado = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.FireTornado"));
	GameplayTags.Ability_HealArea = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.HealArea"));
	GameplayTags.Ability_BlackHole = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.BlackHole"));

	GameplayTags.Cooldown_Ability_ThrowDagger = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ability.ThrowDagger"));
	GameplayTags.Cooldown_Ability_TheWorld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ability.TheWorld"));
	GameplayTags.Cooldown_Ability_ShootCable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ability.ShootCable"));
	GameplayTags.Cooldown_Ability_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ability.Lightning"));
	GameplayTags.Cooldown_Ability_FireTornado = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ability.FireTornado"));
	GameplayTags.Cooldown_Ability_HealArea = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ability.HealArea"));
	GameplayTags.Cooldown_Ability_BlackHole = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Ability.BlackHole"));

	GameplayTags.Ability_Enemy_Spawn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Enemy.Spawn"));
	GameplayTags.Ability_Enemy_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Enemy.Melee"));
	GameplayTags.Ability_Enemy_RedBuff_Uppercut = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Enemy.RedBuff.Uppercut"));
	GameplayTags.Ability_Enemy_RedBuff_BigSmash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Enemy.RedBuff.BigSmash"));
	GameplayTags.Ability_Enemy_BlackBuff_SpitFireBall = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Enemy.BlackBuff.SpitFireBall"));

	GameplayTags.Event_Enemy_ComboMelee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Enemy.ComboMelee"));
	GameplayTags.Event_Enemy_LeftMelee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Enemy.LeftMelee"));
	GameplayTags.Event_Enemy_RightMelee = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.Enemy.RightMelee"));

	GameplayTags.Weapon_Melee_AttackHeavy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.Melee.AttackHeavy"));
	GameplayTags.Weapon_Melee_AttackLight = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.Melee.AttackLight"));
	GameplayTags.Weapon_Melee_BaseballBat = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.Melee.BaseballBat"));
	GameplayTags.Weapon_Melee_BlueSword = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.Melee.BlueSword"));
	GameplayTags.Weapon_Range_Gun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.Range.Gun"));
	GameplayTags.Weapon_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.None"));

	GameplayTags.Data_Enemy_Level = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Data.Enemy.Level"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"));

	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effect.HitReact"));

	GameplayTags.WeaponTagtoCue.Add(GameplayTags.Weapon_Melee_BaseballBat, GameplayTags.GameplayCue_BaseballBat);
	GameplayTags.WeaponTagtoCue.Add(GameplayTags.Weapon_Melee_BlueSword, GameplayTags.GameplayCue_BlueSword);
	GameplayTags.WeaponTagtoCue.Add(GameplayTags.Weapon_Range_Gun, GameplayTags.GameplayCue_Gun);
	
	GameplayTags.AbilityTagToWeapon.Add(GameplayTags.Ability_BaseBall, GameplayTags.Weapon_Melee_BaseballBat);
	GameplayTags.AbilityTagToWeapon.Add(GameplayTags.Ability_BlueSword, GameplayTags.Weapon_Melee_BlueSword);
	GameplayTags.AbilityTagToWeapon.Add(GameplayTags.Ability_Rifle, GameplayTags.Weapon_Range_Gun);

}
