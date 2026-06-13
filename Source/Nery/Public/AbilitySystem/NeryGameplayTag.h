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

private:
	static  FNeryGameplayTags NeryGameplayTags;
};
