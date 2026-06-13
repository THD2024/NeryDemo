// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NeryGameplayTag.h"


//静态成员类内定义，内外初始化
FNeryGameplayTags FNeryGameplayTags::NeryGameplayTags;

void FNeryGameplayTags::InitializeTags()
{
	//在这里添加需要添加的标签
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Basic.Resilience"), FString(TEXT("Basic Attribute,strengthen Armor")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Basic.Strength"), FString(TEXT("Basic Attribute,strengthen ArmorPresentation,criticalhit")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Basic.Vigor"), FString(TEXT("Basic Attribute,strengthen Health")));

	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Stamina"), FString(TEXT("The Stamina")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"), FString(TEXT("The Armor")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"), FString(TEXT("The ArmorPenetration")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"), FString(TEXT("The CriticalHitChance")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitEffect"), FString(TEXT("The CriticalHitEffect")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"), FString(TEXT("The MaxMana")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"), FString(TEXT("The MaxHealth")));

	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Health"), FString(TEXT("Health")));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Mana"), FString(TEXT("Mana")));
}
