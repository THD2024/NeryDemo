// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include"Data/InputTagToAbilityInfo.h"
#include "NeryInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API UNeryInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template< typename UserClass,typename PressedFuncType,typename HeldFuncType,typename ReleasedFuncType>
	void BindActionAbility(const UInputTagtoAbilityInfo* InputTagConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc);
};

template<typename UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
inline void UNeryInputComponent::BindActionAbility(const UInputTagtoAbilityInfo* InputTagConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
{
	if (InputTagConfig)
	{
		for (auto& Config : InputTagConfig->InputTagtoAbility)
		{
			if (Config.InputAction && Config.InputTag)
			{
				if (PressedFunc)
				{
					BindAction(Config.InputAction, ETriggerEvent::Started, Object, PressedFunc,Config.InputTag);
				}
				if (ReleasedFunc)
				{
					BindAction(Config.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Config.InputTag);
				}
				if (HeldFunc)
				{
					BindAction(Config.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Config.InputTag);
				}
			}
		}
	}
}
