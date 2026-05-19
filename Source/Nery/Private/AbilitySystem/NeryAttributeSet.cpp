// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/NeryAttributeSet.h"
#include"Net/UnrealNetwork.h"//注册到网络复制属性的必要头文件

UNeryAttributeSet::UNeryAttributeSet()
{
	InitHealth(100.f);
}

void UNeryAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNeryAttributeSet, Health, OldHealth);
}

void UNeryAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UNeryAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UNeryAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UNeryAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);
}
