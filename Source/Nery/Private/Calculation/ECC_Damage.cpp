// Fill out your copyright notice in the Description page of Project Settings.


#include "Calculation/ECC_Damage.h"
#include"GameplayTagContainer.h"
#include"AbilitySystem/NeryGameplayTag.h"
#include"AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

struct NeryDamageStatics
{
	/*Capture Attribute*/
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);//敌人护甲
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);//我方
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);//我方
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitEffect);//我方
	/*Capture Attribute*/

	NeryDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UNeryAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UNeryAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UNeryAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UNeryAttributeSet, CriticalHitEffect, Source, false);
	};

};

static const NeryDamageStatics GetDamageStatics()
{//定义全局函数，用于直接获取到静态变量
	static NeryDamageStatics DamageStatics;
	return DamageStatics;
}

UECC_Damage::UECC_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitEffectDef);
}

void UECC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters & ExecutionParams, FGameplayEffectCustomExecutionOutput & OutExecutionOutput) const
{
	//这里不要用Const
	 UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	 UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	FAggregatorEvaluateParameters EvaluateParameters = FAggregatorEvaluateParameters();
	//这里的damage需要通过引进等级来使实现,暂时通过硬编码设置为100
	const FGameplayEffectSpec CurrentSpec = ExecutionParams.GetOwningSpec();
	float Damage = CurrentSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Damage.Normal"));

	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, EvaluateParameters, Armor);
	Armor = FMath::Max(0.f, Armor);
	//Armor = FMath::Clamp(Armor,0.f,Armor);

	float ArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorPenetrationDef, EvaluateParameters, ArmorPenetration);
	ArmorPenetration = FMath::Max(0.f, ArmorPenetration);

	float EffectArmor = Armor * (100.f - ArmorPenetration)/100;
	Damage *= (100.f - EffectArmor) / 100.f;

	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef, EvaluateParameters, CriticalHitChance);
	CriticalHitChance = FMath::Max(0.f, CriticalHitChance);

	float CriticalHitEffect = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitEffectDef, EvaluateParameters, CriticalHitEffect);
	CriticalHitEffect = FMath::Max(0.f, CriticalHitEffect);

	bool bCriticalHit = false;
	bCriticalHit = CriticalHitChance > FMath::RandRange(0.f, 1.f) ? true : false;
	 Damage = bCriticalHit ? Damage * (1 + CriticalHitEffect*2 / 100) : Damage;

	 const FGameplayModifierEvaluatedData EvaluatedData = FGameplayModifierEvaluatedData(UNeryAttributeSet::GetInComingDamageAttribute(), EGameplayModOp::Override, Damage);
	 OutExecutionOutput.AddOutputModifier(EvaluatedData);
	 
	 //GameplayCue来实现不同伤害显示
	 if (TargetASC)
	 {
		 FGameplayCueParameters Parameters;
		 //需要传递的有三个东西，一个是伤害，一个是位置，一个是targetactor
		 Parameters.RawMagnitude = Damage;
		 const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
		 const FGameplayEffectContext* EffectContext = Spec.GetContext().Get();
		 if (EffectContext)
		 {
			 if (EffectContext->GetHitResult())
			 {
				 //这的Impactpoint在效果应用是被添加了。
				 Parameters.Location = EffectContext->GetHitResult()->ImpactPoint;
			 }
		 }
		 
		 const FGameplayTag CueTag = bCriticalHit? FNeryGameplayTags::GetNeryGameplayTags().GameplayCue_Damage_Critical : FNeryGameplayTags::GetNeryGameplayTags().GameplayCue_Damage_Normal;
		 TargetASC->ExecuteGameplayCue(CueTag, Parameters);
	 }
}
