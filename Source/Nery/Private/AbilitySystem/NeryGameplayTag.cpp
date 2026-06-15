// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NeryGameplayTag.h"


//静态成员类内定义，内外初始化
FNeryGameplayTags FNeryGameplayTags::NeryGameplayTags;

void FNeryGameplayTags::InitializeTags()
{
	//在这里添加需要添加的标签
	NeryGameplayTags.Attribute_Basic_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Basic.Resilience"), FString(TEXT("Basic Attribute,strengthen Armor")));
	NeryGameplayTags.Attribute_Basic_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Basic.Strength"), FString(TEXT("Basic Attribute,strengthen ArmorPresentation,criticalhit")));
	NeryGameplayTags.Attribute_Basic_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Basic.Vigor"), FString(TEXT("Basic Attribute,strengthen Health")));

	NeryGameplayTags.Attribute_Secondary_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Stamina"), FString(TEXT("The Stamina")));
	NeryGameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"), FString(TEXT("The Armor")));
	NeryGameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"), FString(TEXT("The ArmorPenetration")));
	NeryGameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChance"), FString(TEXT("The CriticalHitChance")));
	NeryGameplayTags.Attribute_Secondary_CriticalHitEffect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitEffect"), FString(TEXT("The CriticalHitEffect")));
	NeryGameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"), FString(TEXT("The MaxMana")));
	NeryGameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"), FString(TEXT("The MaxHealth")));

	NeryGameplayTags.Attribute_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Health"), FString(TEXT("Health")));
	NeryGameplayTags.Attribute_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Mana"), FString(TEXT("Mana")));
}
