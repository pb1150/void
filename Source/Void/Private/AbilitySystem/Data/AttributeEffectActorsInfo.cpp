// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeEffectActorsInfo.h"

FAttributeActorInfo* UAttributeEffectActorsInfo::FindAttributeActorInfoByTag(const FGameplayTag& Tag, bool bLogNotFound)
{
	for (FAttributeActorInfo& Info : AttributeActorInformation)
	{
		if (Tag.MatchesTagExact(Info.ActorTag))
		{
			return &Info; 
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeEffectActor [%s] on AttributeEffectActorsInfo [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
