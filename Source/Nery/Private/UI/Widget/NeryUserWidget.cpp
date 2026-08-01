// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/NeryUserWidget.h"
#include"UI/Controller/WidgetController.h"

void UNeryUserWidget::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}

void UNeryUserWidget::SetOpenMenuValue()//统一的按键唤醒菜单入口
{
	OnOpenMenuChange();
}

void UNeryUserWidget::UpdateAttributeMenu_Implementation(bool CanOpen)
{
	CanOpenMenu = CanOpen;
	SetOpenMenuValue();
}

void UNeryUserWidget::UpdateAbiltyMenu_Implementation(bool InCanOpenAbilityMenu)
{
	CanOpenAbilityMenu = InCanOpenAbilityMenu;
	OnOpenAbilityMenuChanged();
}


