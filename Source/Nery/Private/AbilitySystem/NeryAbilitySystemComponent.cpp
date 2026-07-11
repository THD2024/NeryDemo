// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NeryAbilitySystemComponent.h"
#include"AbilitySystem/GameplayAbility/NeryGameplayAbility.h"
#include"GameplayAbilitySpec.h"

void UNeryAbilitySystemComponent::GiveCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& BasicAbilitiesClass)
{
	if (!GetOwner()->HasAuthority())return;//同样这里只能服务器来赋予能力
	for (auto& BasicAbilityClass : BasicAbilitiesClass)
	{
		const UNeryGameplayAbility* AbilityCDO = Cast<UNeryGameplayAbility>(BasicAbilityClass.GetDefaultObject());
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(BasicAbilityClass, 1,INDEX_NONE,GetAvatarActor());
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
		for (const auto& AbilitySpec : GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InTag))
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

