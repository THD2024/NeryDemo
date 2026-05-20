// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/NeryHUD.h"
#include"UI/Widget/NeryUserWidget.h"
#include"UI/Controller/OverlayWidgetController.h"


void ANeryHUD::InitWidgetAndController(const FWidgetControllerParams& Params)
{
	InitWidget();
	if (OverlayWidget)
	{
		OverlayWidget->SetWidgetController(GetOverlayWidgetController(Params));
		OverlayWidget->AddToViewport();
	}
}

void ANeryHUD::BeginPlay()
{
	Super::BeginPlay();

}

void ANeryHUD::InitWidget()
{
	if (OverlayWidget == nullptr)
	{
		//显示到世界中，所以第一个参数是GetWorld()，第二个参数是要创建的widget的类
		OverlayWidget = CreateWidget<UNeryUserWidget>(GetWorld(), OverlayWidgetClass);
	}
}



UWidgetController* ANeryHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params) 
{
	if (!OverlayWidgetController)
	{//判断当前的控制器是否是空的，如果是就创建一个控制器并且赋值。
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->InitWidgetController(Params);
	}
	return OverlayWidgetController;
}
