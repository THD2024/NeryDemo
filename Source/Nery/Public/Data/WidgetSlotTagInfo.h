// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "WidgetSlotTagInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FWidgetSlotTag
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TypeTag;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> WidgetSlotTags;

};

UCLASS(BlueprintType,Blueprintable)
class NERY_API UWidgetSlotTagInfo: public UObject
{
	GENERATED_BODY()

public:
	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	const FGameplayTag GetBuffWidgetTag();//这里先就只写获取到buffactorwidget的逻辑

	void SetBuffActorWidgetTagByTag(const FGameplayTag& WidgetTag);//这里先就只写设置到buffactorwidget的逻辑


protected:
	//这三个的key在蓝图中进行填充。因为是固定的，只有value是变化的，通过代码实现变化
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Replicated)
	FWidgetSlotTag BuffSlotTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
	FWidgetSlotTag PhysicalAbilitySlotTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
	FWidgetSlotTag MagicBuffSlotTags;
	
};
