// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/WidgetSlotTagInfo.h"

#include "../../../../../../../source/UE_5.2/Engine/Plugins/Importers/USDImporter/Source/ThirdParty/USD/include/pxr/usd/usdShade/types.h"
#include "../../../../../../../source/UE_5.2/Engine/Plugins/Importers/USDImporter/Source/ThirdParty/USD/include/pxr/usd/usdShade/types.h"
#include"Net/UnrealNetwork.h"


void UWidgetSlotTagInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWidgetSlotTagInfo, BuffSlotTags);
	DOREPLIFETIME(UWidgetSlotTagInfo, PhysicalAbilitySlotTags);
	DOREPLIFETIME(UWidgetSlotTagInfo, MagicAbilitySlotTags);
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

void UWidgetSlotTagInfo::SetMagicAbilitySlotTag(const FGameplayTag& AbilitySlotTag)
{
	if (MagicAbilitySlotTags.TypeTag.MatchesTag(AbilitySlotTag))
	{
		MagicAbilitySlotTags.AbilitySlotTag = AbilitySlotTag;
	}
}

void UWidgetSlotTagInfo::SerPhysicalAbilitySlotTag(const FGameplayTag& InInputTag, const FGameplayTag& AbilitySlotTag)
{
	for (auto& PhysicalAbilitySlotTag : PhysicalAbilitySlotTags)
	{
		if (PhysicalAbilitySlotTag.InputTag.MatchesTagExact(InInputTag))
		{
			PhysicalAbilitySlotTag.AbilitySlotTag = AbilitySlotTag;
		}
	}
}

FGameplayTag UWidgetSlotTagInfo::GetAbilitySlotTagByInputTag(const FGameplayTag& InInputTag)
{
	if (MagicAbilitySlotTags.InputTag.MatchesTagExact(InInputTag))
	{
		return MagicAbilitySlotTags.AbilitySlotTag;
	}
	for (auto& PhysicalAbilitySlotTag : PhysicalAbilitySlotTags)
	{
		if (PhysicalAbilitySlotTag.InputTag.MatchesTag(InInputTag))
		{
			return PhysicalAbilitySlotTag.AbilitySlotTag;
		}
	}
	return FGameplayTag();
}
