// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/AbilityWidgetController.h"

#include "AbilitySystemComponent.h"
#include"Interface/CombatInterface.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"



void UAbilityWidgetController::BroadInitValue()
{
	BroadWidgetAbilityInfo();
}

void UAbilityWidgetController::BindCallBacks()
{
	if (!WidgetSlotTagInfo)
	{
		WidgetSlotTagInfo = UNeryBlueprintFunctionLibrary::GetWidgeetSlotTagInfo(this);
	}
	if (WidgetSlotTagInfo)
	{
		WidgetSlotTagInfo->OnAbilitySlotTagChanged.AddUObject(this, &UAbilityWidgetController::BroadWidgetAbilityInfo);
	}
}

void UAbilityWidgetController::BroadWidgetAbilityInfo()
{	
	if (AbilitySystemComponent->GetAvatarActor() && AbilitySystemComponent->GetAvatarActor()->Implements<UCombatInterface>())
	{
		TArray<FGameplayTag>CharacterOwningAbilities = ICombatInterface::Execute_GetCharacterActivateAbilities(AbilitySystemComponent->GetAvatarActor());
		if (AbilityDataAsset)
		{
			for (auto& OwningAbility : CharacterOwningAbilities)
			{
				if (OwningAbility.IsValid())
				{
					FNeryAbilityInfo AbilityInfo = AbilityDataAsset->GetAbilityInfoByTag(OwningAbility);
					OnWidgetSlotDelegate.Broadcast(AbilityInfo);
				}
			}
		}
	}
}
