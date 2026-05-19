// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WidgetController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

void UWidgetController::InitWidgetController(const FWidgetControllerParams& Params)
{
	PlayerState = Params.PlayerState;
	PlayerController = Params.PlayerController;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}
