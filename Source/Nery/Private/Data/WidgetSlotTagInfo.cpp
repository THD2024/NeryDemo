// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/WidgetSlotTagInfo.h"


const FGameplayTag UWidgetSlotTagInfo::GetBuffWidgetTag()
{
	if (BuffActorWidgetTag.Num() <= 0) return FGameplayTag();
	 auto Info = BuffActorWidgetTag.CreateConstIterator();
	 return Info.Value();
}

void UWidgetSlotTagInfo::SetBuffActorWidgetTag(const FGameplayTag& WidgetTag)
{
	if (BuffActorWidgetTag.Num() <= 0)return;
	for (auto& BuffTagInfo : BuffActorWidgetTag)
	{
		if (WidgetTag.MatchesTag(BuffTagInfo.Key))
		{
			BuffTagInfo.Value = WidgetTag;
		}
	}
}
