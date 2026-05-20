// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WidgetController.generated.h"

/**
 * 
 */

class APlayerState;
class APlayerController;
class UAbilitySystemComponent;
class UAttributeSet;

//将需要的参数打包成一个结构体，方便传递，在具体赋值时，只需要定义一个结构体变量，赋值后直接传递即可
struct FWidgetControllerParams
{
public:
	FWidgetControllerParams(APlayerState* InPlayerState, APlayerController* InPlayerController, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
		: PlayerState(InPlayerState)
		, PlayerController(InPlayerController)
		, AbilitySystemComponent(InAbilitySystemComponent)
		, AttributeSet(InAttributeSet){}
	
	APlayerState* PlayerState = nullptr;
	APlayerController* PlayerController = nullptr;
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;
	UAttributeSet* AttributeSet = nullptr;
};

UCLASS(Blueprintable,BlueprintType)
class NERY_API UWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;

	void InitWidgetController(const FWidgetControllerParams& Params);
};
