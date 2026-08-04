// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NeryHUD.generated.h"

class ANeryPlayerState;
/**
 * 
 */
struct FWidgetControllerParams;//在这里是需要声明父类控制器的参数结构体，这里不需要调用到具体控制器的函数
class UWidgetController;//在这里是需要声明父类控制器，这里不需要调用到具体控制器的函数
class UNeryUserWidget;//在这里是需要声明父类控制器的参数结构体，这里不需要调用到具体控制器的函数
class UAttributeWidgetController;
class UAbilityWidgetController;

UCLASS()
class NERY_API ANeryHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//todo
	void InitWidgetAndController(const FWidgetControllerParams& Params);//在character中调用这个函数来初始化hud
	UAttributeWidgetController* GetAttributeWidgetController(const FWidgetControllerParams& Params);
	UAbilityWidgetController* GetAbilityWidgetController(const FWidgetControllerParams& Params);
	UNeryUserWidget* GetOverlayWidget();
	//属性菜单的生成在蓝图中进行，不在这里。因为属性菜单是一中动态生成的，不想血量。所以控制器的分配在蓝图中，不在代码中
protected:
	void BeginPlay() override;

	void InitWidget();

	void InitAbilityWidgetController(ANeryPlayerState* PS);
	
	UWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);
	
	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	TObjectPtr<UNeryUserWidget> OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	TObjectPtr<UWidgetController> OverlayWidgetController;//预备第一个子控制器，将基本属性显示到Hud的子控制器

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	TObjectPtr<UAttributeWidgetController> AttributeWidgetController;
	
	UPROPERTY(EditDefaultsOnly,Category = "Controller")
	TObjectPtr<UAbilityWidgetController> AbilityWidgetController;
	
	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	TSubclassOf<UNeryUserWidget> OverlayWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	TSubclassOf<UWidgetController> OverlayWidgetControllerClass;//预备第一个子控制器的类,用来动态创建子控制器
	
	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	TSubclassOf<UAttributeWidgetController> AttributeWidgetControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	TSubclassOf<UAbilityWidgetController> AbilityWidgetControllerClass;
};
