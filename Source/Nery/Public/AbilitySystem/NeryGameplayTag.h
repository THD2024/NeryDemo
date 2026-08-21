// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"GameplayTagsManager.h"
/**
 * 
 */
struct FNeryGameplayTags
{

public:
	static const FNeryGameplayTags& GetNeryGameplayTags() { return NeryGameplayTags; }
	static void InitializeTags();

	/*Basic Attribute*/
	FGameplayTag Attribute_Basic_Resilience;
	FGameplayTag Attribute_Basic_Strength;
	FGameplayTag Attribute_Basic_Vigor;

	/*Secondary Attribute*/
	FGameplayTag Attribute_Secondary_Stamina;
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitEffect;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_MaxHealth;

	/*Vital Attribute*/
	FGameplayTag Attribute_Vital_Health;
	FGameplayTag Attribute_Vital_Mana;

	/*AttributePoint*/
	FGameplayTag Attribute_Level_AttributePoint;

	/*Enemy Attribute*/
	FGameplayTag Attribute_Enemy_Poise;
	FGameplayTag Attribute_Enemy_MaxPoise;
	
	/*GameplayCue*/
	FGameplayTag GameplayCue_Damage_Critical;
	FGameplayTag GameplayCue_Damage_Normal;
	FGameplayTag GameplayCue_Damage_Environmental;
	FGameplayTag GameplayCue_Buff_Health;
	FGameplayTag GameplayCue_Buff_ArmorPenetration;
	FGameplayTag GameplayCue_Buff_CriticalHitChance;
	FGameplayTag GameplayCue_Trap_FireDamage;
	FGameplayTag GameplayCue_AbilityMoon_Begin;//刚释放技能
	FGameplayTag GameplayCue_AbilityMoon_Hit;//刚击中
	FGameplayTag GameplayCue_AbilitySamsara_OuterHit;
	FGameplayTag GameplayCue_AbilitySamsara_InnerHit;
	FGameplayTag GameplayCue_AbilityMagic_ScorchHit;
	FGameplayTag GameplayCue_AbilityGolden_Hit;
	FGameplayTag GameplayCue_AbilityCrescentSalvo_Hit;
	FGameplayTag GameplayCue_AbilityThornMarch_Hit;
	FGameplayTag GameplayCue_AbilityQuakeFang_Hit;
	
	
	/*BuffActor*/
	FGameplayTag Buff_Good_Health;
	FGameplayTag Buff_Good_ArmorPenetration;
	FGameplayTag Buff_Good_CriticalHitChance;
	

	/*Damage*/
	FGameplayTag Damage_Normal;

	/*InputTag*/
	FGameplayTag Input_BasicAttack;
	
	/*EventTag*/
	FGameplayTag Event_AttackInput;
	FGameplayTag Event_AbilityBegin;
	FGameplayTag Event_MontageSectionBegin;
	FGameplayTag Event_TargetInfo;
	FGameplayTag Event_MontageSectionChanged;
	FGameplayTag Event_EventData;
	
	
	/*Ability*/
	FGameplayTag Ability_MoonCrescent;//月光斩
	FGameplayTag Ability_Samsara;//冥紫色轮 
	FGameplayTag Ability_Magic_Scorch;//坠击火焰阵
	FGameplayTag Ability_EnemyBasicAttack;//敌人普攻
	FGameplayTag Ability_GoldenVortex;//金色旋涡
	FGameplayTag Ability_CrescentSalvo;//月光连射
	FGameplayTag Ability_ThornMarch;//地刺行军
	FGameplayTag Ability_QuakeFang;//地突刺
	FGameplayTag Ability_EnemyHitted;//敌人受击
	FGameplayTag Ability_PlayerHitted;//玩家受击

	/*CoolDown*/
	FGameplayTag Ability_CoolDown_Crescent;
	FGameplayTag Ability_CoolDown_Samsara;
	FGameplayTag Ability_CoolDown_Scorch;
	FGameplayTag Ability_CoolDown_GoldenVortex;
	FGameplayTag Ability_CoolDown_EnemyBasicAttack;
	FGameplayTag Ability_CoolDown_ThornMarch;
	FGameplayTag Ability_CoolDown_QuakeFang;
	FGameplayTag Ability_CoolDown_CrescentSalvo;
	
	/*InputTag*/
	FGameplayTag Input_1;
	FGameplayTag Input_2;
	FGameplayTag Input_3;
	
	/*Status*/
	FGameplayTag Status_Enemy_NoPoise;
	FGameplayTag Status_Enemy_Locked;
	FGameplayTag Status_Enemy_UnLocked;
	// FGameplayTag Status_Player_Poise;
	
private:
	static  FNeryGameplayTags NeryGameplayTags;
};
