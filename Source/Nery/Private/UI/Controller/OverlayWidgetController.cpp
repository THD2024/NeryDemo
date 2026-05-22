// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/OverlayWidgetController.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"

void UOverlayWidgetController::BroadInitValue()
{
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		OnAttributeChanged.Broadcast(AS->GetHealth());
	}

}

void UOverlayWidgetController::BindCallBacks()
{
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::OnHealthChanged);
		}
	}
}

void UOverlayWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.NewValue);
}
