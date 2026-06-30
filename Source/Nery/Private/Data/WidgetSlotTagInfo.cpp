// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/WidgetSlotTagInfo.h"
#include"Net/UnrealNetwork.h"


void UWidgetSlotTagInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWidgetSlotTagInfo, BuffSlotTags);
	DOREPLIFETIME(UWidgetSlotTagInfo, PhysicalAbilitySlotTags);
	DOREPLIFETIME(UWidgetSlotTagInfo, MagicBuffSlotTags);
}

const FGameplayTag UWidgetSlotTagInfo::GetBuffWidgetTag()
{
	if (BuffSlotTags.WidgetSlotTags.Num() <= 0) return FGameplayTag();
	return BuffSlotTags.WidgetSlotTags[0];
}

void UWidgetSlotTagInfo::SetBuffActorWidgetTagByTag(const FGameplayTag& WidgetTag)
{
	if (BuffSlotTags.WidgetSlotTags[0] != WidgetTag)
	{
		BuffSlotTags.WidgetSlotTags[0] = WidgetTag;//buffactor从始至终就只有一个显示槽，数据也就只有一个
	}
}
