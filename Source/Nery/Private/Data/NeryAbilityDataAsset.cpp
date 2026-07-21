// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/NeryAbilityDataAsset.h"

FNeryAbilityInfo UNeryAbilityDataAsset::GetAbilityInfoByTag(const FGameplayTag& AbilityTag) const
{
	for (const auto& AbilityInfo :AbilityInfos)
	{
		if(AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}
	return FNeryAbilityInfo();
}

TSubclassOf<UNeryGameplayAbility> UNeryAbilityDataAsset::GetAbilityClassByTag(const FGameplayTag& AbilityTag) const
{
	for (const auto& AbilityInfo :AbilityInfos)
	{
		if(AbilityInfo.AbilityTag == AbilityTag)
		{
			if (AbilityInfo.AbilityClass)
			{
				return AbilityInfo.AbilityClass;
			}
		}
	}
	return TSubclassOf<UGameplayAbility>();
}
