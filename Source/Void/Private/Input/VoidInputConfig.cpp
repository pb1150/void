// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/VoidInputConfig.h"

const UInputAction* UVoidInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound)
{
	for (const FVoidInputAction& Info : AbilityInputActions)
	{
		if (Info.InputTag.MatchesTagExact(InputTag) && Info.InputAction)
		{
			return Info.InputAction;
		}

	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
