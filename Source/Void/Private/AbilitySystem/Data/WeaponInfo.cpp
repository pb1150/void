// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/WeaponInfo.h"

FVoidWeaponInfo UWeaponInfo::FindWeaponInfoByTag(FGameplayTag WeaponTag)
{
	for (const FVoidWeaponInfo& Info : VoidWeaponInformation)
	{
		if (Info.WeaponTag.MatchesTagExact(WeaponTag))
		{
			return Info;
		}
	}
	return FVoidWeaponInfo();
}
