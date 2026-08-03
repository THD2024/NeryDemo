// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/NeryPlayerState.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "VisualLogger/VisualLogger.h"

ANeryPlayerState::ANeryPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UNeryAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UNeryAttributeSet>("AttributeSet");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	NetUpdateFrequency = 100.f;//通用
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());//显式添加属性集子对象到能力系统组件中
}

void ANeryPlayerState::UpdateAbilityWidgetTag(const FGameplayTag& InInputTag, const FGameplayTag& InTag)
{
	for (auto& AbilityWidgetSlotInfo : AbilityWidgetSlotInfos)
	{
		if (AbilityWidgetSlotInfo.InputTag == InInputTag)
		{
			AbilityWidgetSlotInfo.AbilityTag = InTag;
			OnAbilityWidgetChanged.Broadcast();

		}
	}
}

void ANeryPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANeryPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANeryPlayerState,AbilityWidgetSlotInfos);
}

void ANeryPlayerState::OnRep_InputTagMap()
{
	//当abilitywidgetslotinfos更新后，直接广播给控制器
	OnAbilityWidgetChanged.Broadcast();
}

void ANeryPlayerState::UpdateAbilityWidgetSlot_Implementation(const FGameplayTag& InInputTag, const FGameplayTag& InTag)
{
	UpdateAbilityWidgetTag(InInputTag,InTag);
}

const FGameplayTag ANeryPlayerState::GetAbilitySlotTagByInputTag(const FGameplayTag& InInputTag)
{
	for (auto& AbilityWidgetSlotInfo : AbilityWidgetSlotInfos)
	{
		if (AbilityWidgetSlotInfo.InputTag == InInputTag )
		{
			return AbilityWidgetSlotInfo.AbilityTag;
		}
	}
	return FGameplayTag();
}
