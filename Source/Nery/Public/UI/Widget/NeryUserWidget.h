// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include"Interface/CombatInterface.h"
#include "NeryUserWidget.generated.h"


class UWidgetController;

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

	virtual void UpdateAttributeMenu_Implementation(bool CanOpen) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	TObjectPtr<UWidgetController> WidgetController;

	UPROPERTY(BlueprintReadWrite)
	bool CanOpenMenu = false;



};
