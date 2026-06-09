// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NeryAttributeSet.h"
#include"Net/UnrealNetwork.h"//注册到网络复制属性的必要头文件

UNeryAttributeSet::UNeryAttributeSet()
{
	InitHealth(50.0);//初始化属性值
}


void UNeryAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Resilience, OldResilience);
}

void UNeryAttributeSet::OnRep_Strength(const FGameplayAttributeData & OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Strength, OldStrength);
}

void UNeryAttributeSet::OnRep_Vigor(const FGameplayAttributeData & OldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Vigor, OldVigor);

}

void UNeryAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Health, OldHealth);
}

void UNeryAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Stamina, OldStamina);
}

void UNeryAttributeSet::OnRep_Armor(const FGameplayAttributeData & OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Armor, OldArmor);
}

void UNeryAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData & OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UNeryAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData & OldCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UNeryAttributeSet::OnRep_CriticalHitEffect(const FGameplayAttributeData & OldCriticalHitEffect)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, CriticalHitEffect, OldCriticalHitEffect);
}

void UNeryAttributeSet::OnRep_Mana(const FGameplayAttributeData & OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Mana, OldMana);
}

void UNeryAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, MaxMana, OldMaxMana);
}

void UNeryAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData & OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, MaxHealth, OldMaxHealth);
}

void UNeryAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/*Secondery Attribute*/
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, CriticalHitEffect, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	/*Basic Attribute*/
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	
	/*Vital Attribute*/
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UNeryAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UNeryAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);
}
