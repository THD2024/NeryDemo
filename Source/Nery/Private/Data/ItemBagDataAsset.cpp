// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ItemBagDataAsset.h"

void UItemBagDataAsset::AddItem(const FGameplayTag& InTag)
{//拾取到东西后就调用这个函数
	FItemInformation ItemInfo = GetSpecificItemInfoByTag(InTag);
	ItemInfo.StorageNumber++;
}

void UItemBagDataAsset::ReduceItem(const FGameplayTag& InTag)
{//使用了一次效果后就调用这个，用来更新
	FItemInformation ItemInfo = GetSpecificItemInfoByTag(InTag);
	if (ItemInfo.StorageNumber == 0)return;
	ItemInfo.StorageNumber--;
}

TSubclassOf<UGameplayEffect> UItemBagDataAsset::FindSpecificEffectByTag(const FGameplayTag& InTag)
{
	for (const auto& ItemInfo : ItemBag)
	{
		if (InTag.MatchesTag(ItemInfo.BuffTag))
		{
			return ItemInfo.BuffEffect;
		}
	}
	return TSubclassOf<UGameplayEffect>();
}

FItemInformation& UItemBagDataAsset::GetSpecificItemInfoByTag(const FGameplayTag& InTag)
{
	for ( auto& ItemInfo : ItemBag)
	{
		if (InTag.MatchesTag(ItemInfo.BuffTag))
		{
			return ItemInfo;
		}
	}
	return FItemInformation();
}
