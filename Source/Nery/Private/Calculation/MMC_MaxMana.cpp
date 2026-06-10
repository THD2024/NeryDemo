// Fill out your copyright notice in the Description page of Project Settings.


#include "Calculation/MMC_MaxMana.h"
#include"AbilitySystem/NeryAttributeSet.h"


UMMC_MaxMana::UMMC_MaxMana()
{
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;//表示的是spec应用的目标
	VigorDef.AttributeToCapture = UNeryAttributeSet::GetVigorAttribute();
	VigorDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTag;
	EvaluateParams.TargetTags = TargetTag;

	float CurrentVigor;
	//通过这里捕获到需要的属性，并且存入到currentvigor
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParams, CurrentVigor);
	CurrentVigor = FMath::Max(CurrentVigor, 0.f);//保证获取到的vigor有效
	return CurrentVigor * 20;
}
