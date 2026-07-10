// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NeryAbilitySystemComponent.h"
#include"AbilitySystem/GameplayAbility/NeryGameplayAbility.h"
#include"GameplayAbilitySpec.h"

void UNeryAbilitySystemComponent::GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& BasicAbilitiesClass)
{
	if (!GetOwner()->HasAuthority())return;
	for (auto& BasicAbilityClass : BasicAbilitiesClass)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(BasicAbilityClass, 1);
		const UNeryGameplayAbility* BasicAbility =Cast<UNeryGameplayAbility>(AbilitySpec.Ability);
		AbilitySpec.DynamicAbilityTags.AddTag(BasicAbility->InputTag);
		GiveAbility(AbilitySpec);
	}
}
