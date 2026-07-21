// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NeryAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include"AbilitySystem/GameplayAbility/NeryGameplayAbility.h"
#include"GameplayAbilitySpec.h"
#include"AbilitySystem/NeryGameplayTag.h"

void UNeryAbilitySystemComponent::GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& BasicAbilitiesClass)
{
	if (!GetOwner()->HasAuthority())return;//同样这里只能服务器来赋予能力
	for (auto& BasicAbilityClass : BasicAbilitiesClass)
	{
		const UNeryGameplayAbility* AbilityCDO = Cast<UNeryGameplayAbility>(BasicAbilityClass.GetDefaultObject());
		FGameplayAbilitySpec AbilitySpec  = FGameplayAbilitySpec(BasicAbilityClass,1);
		if (AbilityCDO && AbilityCDO->InputTag.IsValid())
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AbilityCDO->InputTag);
			GiveAbility(AbilitySpec);
		}
		
	}
}

void UNeryAbilitySystemComponent::ActiveAbilityByDynamicTag(const FGameplayTag& InTag)
{
	if (GetActivatableAbilities().Num() > 0)
	{
		for (auto& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InTag))
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UNeryAbilitySystemComponent::ActiveCommonAttackAbility(const FGameplayTag& InTag)
{
	// FScopedPredictionWindow PredictionWindow(this, true); 
	if (GetActivatableAbilities().Num() > 0)
	{
		for (auto& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InTag))
			{
				TryActivateAbility(AbilitySpec.Handle);
				
			}
		}
	}
}

void UNeryAbilitySystemComponent::GiveCharacterOwningAbility(const FGameplayTag& InAbilityTag/*用来查询dataasset中的abilityclass*/)
{
	if (TSubclassOf<UNeryGameplayAbility>AbilityClass = AbilityDataAsset->GetAbilityClassByTag(InAbilityTag))
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		AbilitySpec.DynamicAbilityTags.AddTag(InAbilityTag);
		GiveAbility(AbilitySpec);
	}
}

void UNeryAbilitySystemComponent::Server_SentEvent_Implementation(const FGameplayTag& InTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = InTag;
	HandleGameplayEvent(InTag,&Payload);
}



