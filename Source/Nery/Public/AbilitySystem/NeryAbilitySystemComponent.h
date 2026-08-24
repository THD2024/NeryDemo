// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Data/NeryAbilityDataAsset.h"
#include "NeryAbilitySystemComponent.generated.h"


/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class NERY_API UNeryAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	

public:
	void GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& BasicAbilitiesClass);//通过赋予能力，将Inuttag和能力联系起来
	
	void ActiveAbilityByDynamicTag(const FGameplayTag& InTag);
	
	void ActiveCommonAttackAbility(const FGameplayTag& InTag);
	
	void GiveCharacterOwningAbility(const FGameplayTag& InTag);

	bool ActiveEnemyAbilityByDynamicTag(const FGameplayTag& InTag);
	
	UFUNCTION(Server,Reliable)
	void Server_SentEvent(const FGameplayTag& InTag);
	
	void ActiveLevelUpGameplayCue();
	
};
