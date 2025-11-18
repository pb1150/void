// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

FVoidAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogNotFound)
{
	for (const FVoidAttributeInfo& Pair : AttributeInformation)
	{
		if (AttributeTag == Pair.AttributeTag)
		{
			return Pair;
		}

	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for Attribute [%s] on AttributeInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FVoidAttributeInfo();

}
