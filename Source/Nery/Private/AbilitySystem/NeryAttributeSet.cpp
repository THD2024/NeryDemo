

#include "AbilitySystem/NeryAttributeSet.h"
#include"AbilitySystem/NeryGameplayTag.h"
#include "GameplayEffectExtension.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include"GameplayEffect.h"
#include"GameplayEffectTypes.h"
#include "Chaos/Deformable/Utilities.h"
#include "Interface/CombatInterface.h"
#include"Net/UnrealNetwork.h"//注册到网络复制属性的必要头文件


UNeryAttributeSet::UNeryAttributeSet()
{
	FNeryGameplayTags GameplayTags = FNeryGameplayTags::GetNeryGameplayTags();
	InitXp(0);
	InitAttributePoint(0);//这个实现显示到attributemenu上面的
	InitLevel(1);
	InitMaxLevel(5);//这个目前通过手动赋值的方式，比较死板，后面换种方式
	InitNextLevelXp(100.f);
	AttributeToTags.Add(GetResilienceAttribute(), GameplayTags.Attribute_Basic_Resilience);
	AttributeToTags.Add(GetStrengthAttribute(), GameplayTags.Attribute_Basic_Strength);
	AttributeToTags.Add(GetVigorAttribute(), GameplayTags.Attribute_Basic_Vigor);

	AttributeToTags.Add(GetArmorAttribute(), GameplayTags.Attribute_Secondary_Armor);
	AttributeToTags.Add(GetArmorPenetrationAttribute(), GameplayTags.Attribute_Secondary_ArmorPenetration);
	AttributeToTags.Add(GetCriticalHitChanceAttribute(), GameplayTags.Attribute_Secondary_CriticalHitChance);
	AttributeToTags.Add(GetCriticalHitEffectAttribute(), GameplayTags.Attribute_Secondary_CriticalHitEffect);
	AttributeToTags.Add(GetMaxManaAttribute(), GameplayTags.Attribute_Secondary_MaxMana);
	AttributeToTags.Add(GetMaxHealthAttribute(), GameplayTags.Attribute_Secondary_MaxHealth);

	AttributeToTags.Add(GetAttributePointAttribute(),GameplayTags.Attribute_Level_AttributePoint);

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

void UNeryAttributeSet::OnRep_Xp(const FGameplayAttributeData& OldXp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Xp, OldXp);
}

void UNeryAttributeSet::OnRep_AttributePoint(const FGameplayAttributeData & OldAttributePoint)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, AttributePoint, OldAttributePoint);
}

void UNeryAttributeSet::OnRep_Level(const FGameplayAttributeData & OldLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Level, OldLevel);
}

void UNeryAttributeSet::OnRep_MaxLevel(const FGameplayAttributeData& OldMaxLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, MaxLevel, OldMaxLevel);

}

void UNeryAttributeSet::OnRep_NextLevelXp(const FGameplayAttributeData& OldNextLevelXp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, NextLevelXp, OldNextLevelXp);
}

void UNeryAttributeSet::OnRep_Poise(const FGameplayAttributeData& OldPoise)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet,Poise,OldPoise);

}

void UNeryAttributeSet::OnRep_MaxPoise(const FGameplayAttributeData& OldMaxPoise)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet,MaxPoise,OldMaxPoise);
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

	/*Character Attributes*/
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Xp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, AttributePoint, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, MaxLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, NextLevelXp, COND_None, REPNOTIFY_Always);
	
	/*Enemy Attribute*/
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet,Poise,COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet,MaxPoise,COND_None, REPNOTIFY_Always);
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
	if (Attribute == GetPoiseAttribute())
	{
		if (GetOwningAbilitySystemComponent() && GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(FNeryGameplayTags::GetNeryGameplayTags().Status_Enemy_NoPoise))
		{
			if (NewValue <= GetPoise())
			{
				NewValue = GetPoise();
			}
		}
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxPoise());
	}
	
}

void UNeryAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{//将伤害设置到真正的血量上
		float NewHealth = Health.GetBaseValue() - GetInComingDamage();
		SetInComingDamage(0.f);
		NewHealth = FMath::Clamp(NewHealth,0.f, GetMaxHealth());
		SetHealth(NewHealth);
		
	}

	if (Data.EvaluatedData.Attribute == GetInComingXpAttribute())
	{
		//这里面写经验值的处理逻辑，并且计算等级提升和属性点的增加
		//也就是说这里等级从来不会被直接修改，而是通过经验值的增加来间接修改等级
		//而每次升级，都会增加属性点的数量,同时属性点在Ui中显示的时候也应该是Int32
		float NewXp = Xp.GetBaseValue() + GetInComingXp();
		SetInComingXp(0.f);
		NewXp = FMath::Clamp(NewXp, 0.f, GetNextLevelXp());
		SetXp(NewXp);
		if(AActor* Instigator = Data.EffectSpec.GetContext().GetInstigator())
		{
			if (GetLevel() >= GetMaxLevel())return;
			AutoHandleLevelUp(Instigator);
		}
	}
	if (Data.EvaluatedData.Attribute == GetInComingPoiseAttribute())
	{
		if (GetOwningActor() && GetOwningActor()->Implements<UCombatInterface>())
		{
			//该变量表示当前是否在poise恢复阶段
			bool bEnemyPoise = ICombatInterface::Execute_GetEnemyPoiseStatus(GetOwningActor());
			if (!bEnemyPoise)//只有当当前不是在恢复状态情况下才能对Poise进行操作
			{
				float NewPoise = Poise.GetBaseValue() + GetInComingPoise();
				SetInComingPoise(0.f);
				NewPoise = FMath::Clamp(NewPoise, 0.f, GetMaxPoise());
				SetPoise(NewPoise);
			}
			SetInComingPoise(0.f);//保证了元数据的干净
		}
	}
}

void UNeryAttributeSet::AutoHandleLevelUp(const AActor* Instigator)
{
	const float CurrentXp = GetXp();	
	while (CurrentXp >= GetNextLevelXp() )//这里后面设置等级上限后，记得加上等级上限的判断
	{//当当前xp大于了下一级xp，就证明该升级了，同时计算下一级的经验值
		if (GetLevel() >= GetMaxLevel())return;
		int32 CurrentLevel = FMath::FloorToInt(GetLevel());//向下取整
		int32 NextLevel = CurrentLevel + 1;
		NextLevel = FMath::Clamp(NextLevel, 1, GetMaxLevel());//等级上限为100
		SetLevel(NextLevel);//等级升级
		float TempNextLevelXp = UNeryBlueprintFunctionLibrary::GetXpByLevel(Instigator, GetLevel() + 1);
		SetNextLevelXp(TempNextLevelXp);

		//计算升级后增加的属性点数量
		float AddedAttributePoint = UNeryBlueprintFunctionLibrary::GetAttributePointbyCurrentLevel(Instigator, GetLevel());
		float NewAttributePoint = GetAttributePoint() + AddedAttributePoint;
		NewAttributePoint = FMath::Clamp(NewAttributePoint, 0, 100);
		SetAttributePoint(NewAttributePoint);
	}
}
