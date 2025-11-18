// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/InputKeyDisplayData.h"

FName UInputKeyDisplayData::GetDisplayNameForKey(const FName& InKey) const
{
    for (const FKeyDisplayPair& Pair : KeyDisplayMappings)
    {
        if (Pair.KeyName == InKey)
        {
            return Pair.DisplayName;
        }
    }

    return InKey;
}
