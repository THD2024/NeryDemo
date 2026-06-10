// Fill out your copyright notice in the Description page of Project Settings.


#include "Calculation/MMC_MaxHealth.h"
#include"AbilitySystem/NeryAttributeSet.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.AttributeToCapture = UNeryAttributeSet::GetVigorAttribute();
	VigorDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec & Spec) const
{
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTag;
	EvaluateParams.TargetTags = TargetTag;

	float CurrentVigor;
	//通过这里捕获到需要的属性，并且存入到currentvigor
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParams, CurrentVigor);
	CurrentVigor = FMath::Max(CurrentVigor , 0.f);//保证获取到的vigor有效
	return CurrentVigor*20;
}
