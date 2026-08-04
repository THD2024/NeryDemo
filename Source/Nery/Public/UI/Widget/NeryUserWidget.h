// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include"Interface/CombatInterface.h"
#include "NeryUserWidget.generated.h"


class UWidgetController;
class UAbilityWidgetController;

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class NERY_API UNeryUserWidget : public UUserWidget,public ICombatInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UWidgetController* InWidgetController);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

	UFUNCTION(BlueprintCallable)
	void SetOpenMenuValue();

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenMenuChange();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnOpenAbilityMenuChanged();
	
	UFUNCTION(BlueprintImplementableEvent)
	void CanInitAbilityController(UAbilityWidgetController* AbilityWidgetController);

	//接口
	virtual void UpdateAttributeMenu_Implementation(bool CanOpen) override;
	virtual void UpdateAbiltyMenu_Implementation(bool InCanOpenAbilityMenu) override;
	virtual void OnAbilityWidgetControllerSet_Implementation(UAbilityWidgetController* AbilityWidgetController) override;
	//接口
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	TObjectPtr<UWidgetController> WidgetController;

	UPROPERTY(BlueprintReadWrite)
	bool CanOpenMenu = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool CanOpenAbilityMenu = false;//目前就只有一个属性菜单和一个技能菜单，所以这里不扩展写技能唤醒菜单的通用逻辑。这里就两个分开写



};
