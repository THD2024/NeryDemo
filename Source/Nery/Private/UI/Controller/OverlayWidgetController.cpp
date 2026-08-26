// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/OverlayWidgetController.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"PlayerController/NeryPlayerController.h"
#include"Interface/CombatInterface.h"
#include"Data/ItemBagDataAsset.h"

void UOverlayWidgetController::BroadInitValue()
{
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		HealthChanged.Broadcast(AS->GetHealth());
		MaxHealthChanged.Broadcast(AS->GetMaxHealth());
		LevelChanged.Broadcast(AS->GetLevel());
		XpChanged.Broadcast(AS->GetXp());
		NextLevelXpChanged.Broadcast(AS->GetNextLevelXp());
		StaminaChanged.Broadcast(AS->GetStamina());
	}
	BroadBuffInfo();
}

void UOverlayWidgetController::BindCallBacks()
{
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::OnHealthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::OnMaxHealthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetLevelAttribute()).AddUObject(this, &UOverlayWidgetController::OnLevelChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetXpAttribute()).AddUObject(this, &UOverlayWidgetController::OnXpChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetNextLevelXpAttribute()).AddUObject(this, &UOverlayWidgetController::OnNextLevelXpChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetStaminaAttribute()).AddUObject(this, &UOverlayWidgetController::OnStaminaChanged);
		}
	}
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(PlayerController))
	{
		PC->OnConsumeInput.AddUObject(this, &UOverlayWidgetController::OnBuffInfoChanged);
		PC->OnBuffNumberAdded.AddUObject(this, &UOverlayWidgetController::OnBuffInfoChanged);
		PC->OnRightScrollInput.AddUObject(this, &UOverlayWidgetController::OnBroadRightScroll);
		PC->OnLeftScrollInput.AddUObject(this, &UOverlayWidgetController::OnBroadLeftScroll);
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

void UOverlayWidgetController::OnLevelChanged(const FOnAttributeChangeData& Data)
{
	LevelChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnXpChanged(const FOnAttributeChangeData & Data)
{
	XpChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnNextLevelXpChanged(const FOnAttributeChangeData& Data)
{
	NextLevelXpChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	StaminaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnBuffInfoChanged()
{
	//不管三七二十一，全给打包过去。根据当前组件中显示的actor类型来匹配到对应的信息，当然，如果在这之间没有使用药水，但是需要
	// 翻看，则最好的办法就是每次
	//在这里传递广播
	
	BroadBuffInfo();
	
}

void UOverlayWidgetController::OnBroadRightScroll()
{
	RightScroll.Broadcast();
}

void UOverlayWidgetController::OnBroadLeftScroll()
{
	LeftScroll.Broadcast();
}

void UOverlayWidgetController::BroadBuffInfo()
{
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(PlayerController))
	{
		if (PC->GetPawn() && PC->GetPawn()->Implements<UCombatInterface>())
		{
			APawn* Pawn = PC->GetPawn();
			FBuffNumberInfo PassBuffInfo;
			PassBuffInfo.NewBuffInfo = ICombatInterface::Execute_GetBuffNumber(Pawn);
			//每次将这个标签和数量全部打包过去
			BuffInfoChanged.Broadcast(PassBuffInfo);

		}
	}
}
