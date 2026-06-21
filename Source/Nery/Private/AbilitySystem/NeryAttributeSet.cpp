

#include "AbilitySystem/NeryAttributeSet.h"
#include"AbilitySystem/NeryGameplayTag.h"
#include"Net/UnrealNetwork.h"//注册到网络复制属性的必要头文件

UNeryAttributeSet::UNeryAttributeSet()
{
	FNeryGameplayTags GameplayTags = FNeryGameplayTags::GetNeryGameplayTags();
	AttributeToTags.Add(GetResilienceAttribute(), GameplayTags.Attribute_Basic_Resilience);
	AttributeToTags.Add(GetStrengthAttribute(), GameplayTags.Attribute_Basic_Strength);
	AttributeToTags.Add(GetVigorAttribute(), GameplayTags.Attribute_Basic_Vigor);

	AttributeToTags.Add(GetArmorAttribute(), GameplayTags.Attribute_Secondary_Armor);
	AttributeToTags.Add(GetArmorPenetrationAttribute(), GameplayTags.Attribute_Secondary_ArmorPenetration);
	AttributeToTags.Add(GetCriticalHitChanceAttribute(), GameplayTags.Attribute_Secondary_CriticalHitChance);
	AttributeToTags.Add(GetCriticalHitEffectAttribute(), GameplayTags.Attribute_Secondary_CriticalHitEffect);
	AttributeToTags.Add(GetMaxManaAttribute(), GameplayTags.Attribute_Secondary_MaxMana);
	AttributeToTags.Add(GetMaxHealthAttribute(), GameplayTags.Attribute_Secondary_MaxHealth);
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
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	if (Attribute == GetArmorAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	if (Attribute == GetArmorPenetrationAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
	if (Attribute == GetCriticalHitChanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	if (Attribute == GetCriticalHitEffectAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
}

void UNeryAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);
	
}
