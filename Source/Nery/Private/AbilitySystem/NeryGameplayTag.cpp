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

	NeryGameplayTags.Attribute_Level_AttributePoint = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Level.AttributePoint"), FString(TEXT("AttributePoint")));

	NeryGameplayTags.GameplayCue_Damage_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Damage.Normal"), FString(TEXT("Normal Damage")));
	NeryGameplayTags.GameplayCue_Damage_Critical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Damage.Critical"), FString(TEXT("Critical Damage")));
	NeryGameplayTags.GameplayCue_Damage_Environmental = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Damage.Environmental"), FString(TEXT("Environmental Damage")));
	NeryGameplayTags.GameplayCue_Buff_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Buff.ArmorPenetration"), FString(TEXT("Strength your ArmorPenetration")));
	NeryGameplayTags.GameplayCue_Buff_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Buff.CriticalHitChance"), FString(TEXT("Be Easier to Criticalhit")));
	NeryGameplayTags.GameplayCue_Buff_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Buff.Health"), FString(TEXT("Recover your Health")));
	NeryGameplayTags.GameplayCue_Trap_FireDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.Trap.FireDamage"), FString(TEXT("Fire Damage")));
	NeryGameplayTags.GameplayCue_AbilityMoon_Begin = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.AbilityMoon.Begin"),FString(TEXT("Begin")));
	NeryGameplayTags.GameplayCue_AbilityMoon_Hit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.AbilityMoon.Hit"),FString(TEXT("Hit")));
	NeryGameplayTags.GameplayCue_AbilitySamsara_OuterHit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.AbilitySamsara.Outerhit"),FString(TEXT("OuterHit")));
	NeryGameplayTags.GameplayCue_AbilitySamsara_InnerHit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.AbilitySamsara.InnerHit"),FString(TEXT("InnerHit")));
	NeryGameplayTags.GameplayCue_AbilityMagic_ScorchHit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.AbilityMagic.ScorchHit"),FString(TEXT("when Scorch Hits")));
	
	NeryGameplayTags.Buff_Good_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Buff.Good.ArmorPenetration"), FString(TEXT("Strengthen ArmorPenetration")));
	NeryGameplayTags.Buff_Good_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Buff.Good.Health"), FString(TEXT("Recover Health")));
	NeryGameplayTags.Buff_Good_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Buff.Good.CriticalHitChance"), FString(TEXT("Strengthen CriticalHitChance")));

	NeryGameplayTags.Damage_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Normal"), FString(TEXT("Normal Damage")));
	
	NeryGameplayTags.Input_BasicAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.BasicAttack"), FString(TEXT("BasicAttackInputTag")));
	
	NeryGameplayTags.Event_AttackInput = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.AttackInput"), FString(TEXT("AttackInputEvent")));
	NeryGameplayTags.Event_AbilityBegin = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Event.AbilityBegin"),FString(TEXT("First Moment to Spawn Niagara")));
	
	NeryGameplayTags.Ability_MoonCrescent = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.MoonCrescent"),FString(TEXT("First Ability")));
	NeryGameplayTags.Ability_Samsara = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Samsara"),FString(TEXT("Second Ability")));
	NeryGameplayTags.Ability_Magic_Scorch = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Magic.Scorch"),FString(TEXT("Magic Ability")));
}
