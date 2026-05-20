// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NeryUserWidget.generated.h"

class UWidgetController;
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class NERY_API UNeryUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UWidgetController* InWidgetController);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Controller")
	TObjectPtr<UWidgetController> WidgetController;
};
