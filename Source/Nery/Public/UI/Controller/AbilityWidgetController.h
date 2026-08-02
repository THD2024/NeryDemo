// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/WidgetSlotTagInfo.h"
#include "UI/Controller/WidgetController.h"
#include"Data/NeryAbilityDataAsset.h"
#include "AbilityWidgetController.generated.h"

/**
 当前控制器的作用有两个
 一个是要传递常驻界面的技能ui显示信息
 需要时刻盯着插槽技能变化，当这里面一变，马上就要更新，也就是需要时刻盯着widgetslottaginfo这个资产
 ，当对应的Input栏下面的abilitytag一变就需要马上从技能池中获取信息来传递到ui
 
 另一个是要传递当玩家打开装备菜单时，需要将所有的玩家解锁的技能信息传递过去，用来装备到对应的插槽
 这个要复杂点，这个暂时先放着，先把上面那个技能显示做了，这个涉及到更换技能现在完整思路还没有想出来。
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityWidgetDelegate,FNeryAbilityInfo,AbilityInfo);


UCLASS()
class NERY_API UAbilityWidgetController : public UWidgetController
{
	GENERATED_BODY()
	
public:
	
	virtual void BroadInitValue() override;
	
	virtual void BindCallBacks() override;
	
	void BroadInfoByDelegateType(FOnAbilityWidgetDelegate DelegateType);

protected:
	
	UPROPERTY(EditDefaultsOnly,Category = "AbiityAsset")
	TObjectPtr<UNeryAbilityDataAsset> AbilityDataAsset;
	
	UPROPERTY(BlueprintAssignable,Category = "AbilityChanged")
	FOnAbilityWidgetDelegate OnWidgetSlotDelegate;
	
	UPROPERTY(BlueprintAssignable,Category = "AbilityChanged")
	FOnAbilityWidgetDelegate OnAbilityMenuDelegate;
	
	
	void BroadWidgetAbilityInfo();
	
	UFUNCTION(BlueprintCallable)
	void BroadAbilityMenuInfo();//手动调用
};
