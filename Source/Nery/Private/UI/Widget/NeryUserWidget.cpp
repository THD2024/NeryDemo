// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/NeryUserWidget.h"
#include"UI/Controller/WidgetController.h"

void UNeryUserWidget::SetWidgetController(UWidgetController* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}


