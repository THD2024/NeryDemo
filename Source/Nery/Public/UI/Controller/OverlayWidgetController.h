// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/WidgetController.h"
#include"Data/ItemBagDataAsset.h"
#include"GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FBuffNumberInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<FGameplayTag, int32>NewBuffInfo;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged,float, AttributeValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuffInfoChanged, FBuffNumberInfo, BuffNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScrollInput);

/**
 * 
 */

struct FOnAttributeChangeData;

UCLASS()
class NERY_API UOverlayWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:

	void BroadInitValue() override;

	void BindCallBacks() override;

protected:
	/*Delegate*/
	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegate")
	FOnAttributeChanged HealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegate")
	FOnAttributeChanged MaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegate")
	FOnBuffInfoChanged BuffInfoChanged;

	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegate")
	FOnScrollInput RightScroll;

	UPROPERTY(BlueprintAssignable, Category = "AttributeDelegate")
	FOnScrollInput LeftScroll;

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void OnBuffInfoChanged();
	void OnBroadRightScroll();
	void OnBroadLeftScroll();
	void BroadBuffInfo();

};
