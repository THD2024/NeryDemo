// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/AbilityWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/NeryGameplayTag.h"
#include"Interface/CombatInterface.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include"AbilitySystemBlueprintLibrary.h"



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

void UAbilityWidgetController::BroadInfoByDelegateType(FOnAbilityWidgetDelegate DelegateType)
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
					DelegateType.Broadcast(AbilityInfo);
				}
			}
		}
	}
}



void UAbilityWidgetController::BroadWidgetAbilityInfo()
{
	BroadInfoByDelegateType(OnWidgetSlotDelegate);
}

void UAbilityWidgetController::BroadAbilityMenuInfo()
{
	//将技能菜单和装备技能分开，这样当在技能菜单中分配技能菜单不会直接影响到装备技能，当装备后，因为技能插槽中标签变了，这样刚好就会重新广播装备技能信息
	BroadInfoByDelegateType(OnAbilityMenuDelegate);
}
