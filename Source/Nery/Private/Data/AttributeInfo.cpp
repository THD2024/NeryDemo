// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AttributeInfo.h"

const  FNeryAttributeInfo UAttributeInfo::GetAttributeInfoByTag(const FGameplayTag& GameplayTag, bool bIsNotFound)
{
	// TODO: 在此处插入 return 语句
	for (const auto& Info : AttributeInfos)
	{
		if (GameplayTag == Info.AttributeTag)
		{
			return Info;
		}
	}

	if (bIsNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find Info by Tag:[%s] on AttributeInfo"),*GameplayTag.GetTagName().ToString());
	}
	return FNeryAttributeInfo();
}
