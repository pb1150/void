// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "VoidAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FVoidGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	FORCEINLINE bool IsBlockedHit() const { return bIsBlockedHit; }
	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }
	FORCEINLINE bool IsHeavyAttack() const { return bHeavyAttack; }
	FORCEINLINE bool IsCanHitReact() const { return CanHitReact; }

	void SetIsBlockedHit(bool Value) { bIsBlockedHit = Value; }
	void SetIsCriticalHit(bool Value) { bIsCriticalHit = Value; }
	void SetIsHeavyAttack(bool Value) { bHeavyAttack = Value; }
	void SetCanHitReact(bool Value) { CanHitReact = Value; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FVoidGameplayEffectContext* Duplicate() const
	{
		FVoidGameplayEffectContext* NewContext = new FVoidGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}


protected:

	UPROPERTY()
	bool CanHitReact = false;

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bHeavyAttack = false;

};

template<>
struct TStructOpsTypeTraits<FVoidGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FVoidGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};