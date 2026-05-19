// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/NeryPlayerState.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"AbilitySystem/NeryAttributeSet.h"

ANeryPlayerState::ANeryPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UNeryAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UNeryAttributeSet>("AttributeSet");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	NetUpdateFrequency = 100.f;//通用
}

void ANeryPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
}
