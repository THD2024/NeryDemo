// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include "ECC_Damage.generated.h"

/**
 * 
 */


UCLASS()
class NERY_API UECC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UECC_Damage();

	//必要继承函数
	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;


	
};
