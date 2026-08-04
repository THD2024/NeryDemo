// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/NeryHUD.h"

#include "Character/NeryCharacter.h"
#include "PlayerController/NeryPlayerController.h"
#include "PlayerState/NeryPlayerState.h"
#include "UI/Controller/AbilityWidgetController.h"
#include"UI/Widget/NeryUserWidget.h"
#include"UI/Controller/OverlayWidgetController.h"
#include"UI/Controller/AttributeWidgetController.h"




void ANeryHUD::InitWidgetAndController(const FWidgetControllerParams& Params)
{
	InitWidget();
	if (OverlayWidget)
	{
		OverlayWidget->SetWidgetController(GetOverlayWidgetController(Params));
		OverlayWidget->AddToViewport();
		OverlayWidgetController->BindCallBacks();
		OverlayWidgetController->BroadInitValue();
	}
}


void ANeryHUD::BeginPlay()
{
	Super::BeginPlay();
	if (ANeryCharacter* NeryCH = Cast<ANeryCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		NeryCH->OnRepPlayerStateSetted.AddUObject(this, &ANeryHUD::InitAbilityWidgetController);
	}
}

void ANeryHUD::InitWidget()
{
	if (OverlayWidget == nullptr)
	{
		check(OverlayWidgetClass);
		OverlayWidget = CreateWidget<UNeryUserWidget>(GetOwningPlayerController(), OverlayWidgetClass);
	}
}

void ANeryHUD::InitAbilityWidgetController(ANeryPlayerState* PS)
{
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(GetOwningPlayerController()))
	{
		// UNeryBlueprintFunctionLibrary::GetAbilityWidgetController(this,PlayerController);
		if (PS)
		{
			UAbilitySystemComponent* ASC = PS->AbilitySystemComponent;
			UAttributeSet* AS = PS->AttributeSet;
			FWidgetControllerParams Params(PS,PC,ASC,AS);
			 AbilityWidgetController = GetAbilityWidgetController(Params);
			if (GetOverlayWidget()->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_OnAbilityWidgetControllerSet(GetOverlayWidget(),AbilityWidgetController);
			}
			AbilityWidgetController->BroadInitValue();
			
		}
	}
}



UWidgetController* ANeryHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params) 
{
	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->InitWidgetController(Params);
	}
	return OverlayWidgetController;
}

UAbilityWidgetController* ANeryHUD::GetAbilityWidgetController(const FWidgetControllerParams& Params)
{//这个在这里的意义是便于通过蓝图函数库调用这个来直接为该控制器赋值
	if (!AbilityWidgetController)
	{
		AbilityWidgetController = NewObject<UAbilityWidgetController>(this, AbilityWidgetControllerClass);
		AbilityWidgetController->InitWidgetController(Params);
		AbilityWidgetController->BindCallBacks();
	}
	return AbilityWidgetController;
}

UAttributeWidgetController* ANeryHUD::GetAttributeWidgetController(const FWidgetControllerParams& Params)
{
	if (!AttributeWidgetController)
	{
		AttributeWidgetController = NewObject<UAttributeWidgetController>(this, AttributeWidgetControllerClass);
		AttributeWidgetController->InitWidgetController(Params);
		AttributeWidgetController->BindCallBacks();
	}
	return AttributeWidgetController;

}

UNeryUserWidget* ANeryHUD::GetOverlayWidget()
{
	if (OverlayWidget)return OverlayWidget; 
	return nullptr;
}
