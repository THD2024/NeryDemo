// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include"AbilitySystemComponent.h"//获取属性值时需要用到的头文件,被通知和注册属性复制时也需要用到
#include "NeryAttributeSet.generated.h"


struct FGameplayEffectModCallbackData;

//内置的get set函数宏，分别是获取属性的属性，获取属性值，设置属性值，初始化属性值
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)	

/**
 * 
 */
UCLASS()
class NERY_API UNeryAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UNeryAttributeSet();

	TMap<FGameplayAttribute, FGameplayTag> AttributeToTags;

	//属性访问器宏，传入类名和属性名
	/*Basic Attribute*/
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, Resilience);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, Strength);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, Vigor);
	/*Basic Attribute*/
	
	/*Secondary Attribute*/
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, Armor);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, ArmorPenetration);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, CriticalHitChance);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, CriticalHitEffect);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, MaxMana);
	/*Secondary Attribute*/

	/*Vital Attribute*/
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, Health);
	/*Vital Attribute*/

	/*MetaAttribute*/
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, InComingDamage);
	/*MetaAttribute*/
	

	/*Basic Attribute*/
	UPROPERTY(ReplicatedUsing = OnRep_Resilience)
	FGameplayAttributeData Resilience;//减少所受伤害,提高护甲效果

	UPROPERTY(ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;//提高攻击伤害，减小体力消耗

	UPROPERTY(ReplicatedUsing = OnRep_Vigor)
	FGameplayAttributeData Vigor;//增强体力恢复速度和血量上限
	/*Basic Attribute*/

	/*SecondAttribute*/
	
	UPROPERTY(ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;//体力值

	UPROPERTY(ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;//护甲值

	UPROPERTY(ReplicatedUsing = OnRep_ArmorPenetration)
	FGameplayAttributeData ArmorPenetration;//护甲穿透

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitChance)
	FGameplayAttributeData CriticalHitChance;//暴击率

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitEffect)
	FGameplayAttributeData CriticalHitEffect;//暴击效果

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	/*SecondAttribute*/

	/*Vital Attribute*/
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;//生命值

	UPROPERTY(ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	/*Vital Attribute*/

	/*MetaAttribute*/
	UPROPERTY(BlueprintReadOnly, Category = "MetaAttribute")
	FGameplayAttributeData InComingDamage;

	/*MetaAttribute*/


protected:
	/*Delegate Call back Function*/

	/*Basic Attribute*/
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength);

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor);
	/*Basic Attribute*/


	/*Secondery Attribute*/
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance);

	UFUNCTION()
	void OnRep_CriticalHitEffect(const FGameplayAttributeData& OldCriticalHitEffect);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	/*Secondery Attribute*/

	/*Vital Attribute*/

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana);

	/*Vital Attribute*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

};
