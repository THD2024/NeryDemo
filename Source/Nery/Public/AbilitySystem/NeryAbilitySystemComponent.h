// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NeryAbilitySystemComponent.generated.h"


/**
 * 
 */
UCLASS()
class NERY_API UNeryAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	

public:
	void GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& BasicAbilitiesClass);//通过赋予能力，将Inuttag和能力联系起来
	
	void ActiveAbilityByDynamicTag(const FGameplayTag& InTag);
};
