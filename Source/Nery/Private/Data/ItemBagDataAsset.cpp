// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemBagDataAsset.h"

TSubclassOf<UGameplayEffect> UItemBagDataAsset::FindSpecificEffectByTag(const FGameplayTag& InTag)
{
	for (const auto& ItemInfo : ItemBag)
	{
		if (InTag.MatchesTagExact(ItemInfo.BuffTag))
		{
			return ItemInfo.BuffEffect;
		}
	}
	return TSubclassOf<UGameplayEffect>();
}

FItemInformation UItemBagDataAsset::GetSpecificItemInfoByTag(const FGameplayTag& InTag)
{
	for ( auto& ItemInfo : ItemBag)
	{
		if (InTag.MatchesTagExact(ItemInfo.BuffTag))
		{
			return ItemInfo;
		}
	}
	return FItemInformation();//这里是返回的副本，正好用来传递信息到widget
}
