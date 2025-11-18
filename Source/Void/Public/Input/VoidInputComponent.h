//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "VoidInputConfig.h"
#include "VoidInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class VOID_API UVoidInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	template<class UserClass, class PressedFuncType, class ReleasedFuncType, class HeldFuncType>
	void BindAbilityActions(const UVoidInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

 template<class UserClass, class PressedFuncType, class ReleasedFuncType, class HeldFuncType>
 inline void UVoidInputComponent::BindAbilityActions(const UVoidInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
 {
	 check(InputConfig);

	 for (const FVoidInputAction& Action : InputConfig->AbilityInputActions)
	 {
		 if (Action.InputAction && Action.InputTag.IsValid())
		 {
			 if (PressedFunc)
			 {
				 BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			 }
			 if (ReleasedFunc)
			 {
				 BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			 }
			 if (HeldFunc)
			 {
				 BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			 }
			
		 }
		

	 }
 }
