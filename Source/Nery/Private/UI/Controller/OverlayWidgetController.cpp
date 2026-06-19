// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/OverlayWidgetController.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"

void UOverlayWidgetController::BroadInitValue()
{
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		HealthChanged.Broadcast(AS->GetHealth());
		MaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}

}

void UOverlayWidgetController::BindCallBacks()
{
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::OnHealthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::OnMaxHealthChanged);
		}
	}
}

void UOverlayWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	HealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	MaxHealthChanged.Broadcast(Data.NewValue);
}
