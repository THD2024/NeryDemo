// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/InputTagtoAbilityInfo.h"

FInputTagAbilityInfo UInputTagtoAbilityInfo::GetInputTagAbilityInfoByTag(const FGameplayTag& InputTag)
{
	for (const auto& Action : InputTagtoAbility)
	{
		if (Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action;
		}
	}
	return FInputTagAbilityInfo();
}
