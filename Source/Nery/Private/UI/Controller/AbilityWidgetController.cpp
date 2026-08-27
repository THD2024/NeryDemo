// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/AbilityWidgetController.h"
#include "AbilitySystem/NeryGameplayTag.h"
#include"Interface/CombatInterface.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include"AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/NeryAbilitySystemComponent.h"
#include "AbilitySystem/NeryAttributeSet.h"
#include "PlayerState/NeryPlayerState.h"


class UNeryAbilitySystemComponent;
class UNeryAttributeSet;

void UAbilityWidgetController::BroadInitValue()
{
	BroadWidgetAbilityInfo();
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		OnMagicEnergyDelegate.Broadcast(AS->GetMagicEnergy());
	}
}

void UAbilityWidgetController::BindCallBacks()
{
	if (ANeryPlayerState* PS = Cast<ANeryPlayerState>(PlayerState))
	{
		PS->OnAbilityWidgetChanged.AddUObject(this,&UAbilityWidgetController::BroadWidgetAbilityInfo);
	}
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMagicEnergyAttribute()).AddUObject(this, &UAbilityWidgetController::OnMagicEnergyChanged);
			
		}
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
	BroadAbilityMenuInfo();//为什么保证当插槽变更后，不仅技能插槽显示变化，而且技能菜单的分配插槽照样显示变化
}

void UAbilityWidgetController::BroadAbilityMenuInfo()//第一次技能菜单创建手动广播
{
	//将技能菜单和装备技能分开，这样当在技能菜单中分配技能菜单不会直接影响到装备技能，当装备后，因为技能插槽中标签变了，这样刚好就会重新广播装备技能信息
	BroadInfoByDelegateType(OnAbilityMenuDelegate);
}

void UAbilityWidgetController::OnMagicEnergyChanged(const FOnAttributeChangeData& Data)
{
	OnMagicEnergyDelegate.Broadcast(Data.NewValue);
}
