// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/WidgetController.h"
#include "AttributeWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInformation, float, AttributeValue);

/**
 * 
 */
UCLASS()
class NERY_API UAttributeWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:

	void BroadInitValue() override;

	void BindCallBacks() override;

protected:

	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegate")
	FOnAttributeInformation OnAttributeValueChanged;//用来广播属性到属性菜单中

	/*BasicAttribute*/
	void OnResilienceChanged(const FOnAttributeChangeData& Data);
	void OnStrengthChanged(const FOnAttributeChangeData& Data);
	void OnVigorChanged(const FOnAttributeChangeData& Data);

	/*SecondaryAttribute*/
	void OnArmorChanged(const FOnAttributeChangeData& Data);
	void OnArmorPenetrationChanged(const FOnAttributeChangeData& Data);
	void OnCriticalHitChanceChanged(const FOnAttributeChangeData& Data);
	void OnCriticalHitEffectChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxManaChanged(const FOnAttributeChangeData& Data);
};
