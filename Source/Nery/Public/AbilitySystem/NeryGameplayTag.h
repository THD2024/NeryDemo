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

	/*GameplayCue*/
	FGameplayTag GameplayCue_Damage_Critical;
	FGameplayTag GameplayCue_Damage_Normal;
	FGameplayTag GameplayCue_Damage_Environmental;
	FGameplayTag GameplayCue_Buff_Health;
	FGameplayTag GameplayCue_Buff_ArmorPenetration;
	FGameplayTag GameplayCue_Buff_CriticalHitChance;
	FGameplayTag GameplayCue_Trap_FireDamage;

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
	
	/*Ability*/

	

private:
	static  FNeryGameplayTags NeryGameplayTags;
};
