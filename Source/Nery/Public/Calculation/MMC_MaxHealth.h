// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxHealth();

	//必须继承的虚函数，计算过程放到这里面
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const  override;

private:
	FGameplayEffectAttributeCaptureDefinition VigorDef;//定义捕获属性
};
