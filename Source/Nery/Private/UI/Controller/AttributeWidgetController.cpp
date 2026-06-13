// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Controller/AttributeWidgetController.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"

void UAttributeWidgetController::BroadInitValue()
{
	//在这里广播初始值
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{//最开始直接将attributetotags中数组里面的所有属性全部广播
		for (auto& Info : AS->AttributeToTags)
		{
			BroadCastInfo(Info.Key, Info.Key.GetNumericValue(AS));
		}
	}

}

void UAttributeWidgetController::BindCallBacks()
{
	//这里绑定到属性变化回调
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnResilienceChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnStrengthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnVigorChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnArmorChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnArmorPenetrationChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnCriticalHitChanceChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnCriticalHitEffectChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnMaxHealthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this,&UAttributeWidgetController::OnMaxManaChanged);
		}
	}
	
}

void UAttributeWidgetController::OnResilienceChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute,Data.NewValue);
}

void UAttributeWidgetController::BroadCastInfo(const FGameplayAttribute& AttributeData, float NewValue)
{
	if (UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
	{//获取到对应的标签
		if (!AttributeInfo)return;
		FGameplayTag* AttributeTag = AS->AttributeToTags.Find(AttributeData);
		//通过标签获取到attributeinfo
		FNeryAttributeInfo Info = AttributeInfo->GetAttributeInfoByTag(*AttributeTag);
		//为attributeinfo填充最后一个数据后就打包发走
		Info.AttributeValue = NewValue;
		OnAttributeInformationChanged.Broadcast(Info);
	}
}

void UAttributeWidgetController::OnStrengthChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute, Data.NewValue);
}

void UAttributeWidgetController::OnVigorChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute, Data.NewValue);
}

void UAttributeWidgetController::OnArmorChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute, Data.NewValue);
}

void UAttributeWidgetController::OnArmorPenetrationChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute, Data.NewValue);
}

void UAttributeWidgetController::OnCriticalHitChanceChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute, Data.NewValue);
}

void UAttributeWidgetController::OnCriticalHitEffectChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute, Data.NewValue);
}

void UAttributeWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute, Data.NewValue);
}

void UAttributeWidgetController::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
	BroadCastInfo(Data.Attribute, Data.NewValue);
}
