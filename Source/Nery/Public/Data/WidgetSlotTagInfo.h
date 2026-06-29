// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"GameplayTagContainer.h"
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
	const FGameplayTag GetBuffWidgetTag();//这里先就只写获取到buffactorwidget的逻辑

	void SetBuffActorWidgetTag(const FGameplayTag& WidgetTag);//这里先就只写设置到buffactorwidget的逻辑

protected:
	//这三个的key在蓝图中进行填充。因为是固定的，只有value是变化的，通过代码实现变化
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FWidgetSlotTag BuffSlotTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FWidgetSlotTag PhysicalAbilitySlotTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FWidgetSlotTag MagicBuffSlotTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, FGameplayTag>BuffActorWidgetTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, FGameplayTag>PhysicalAbilityWidgetTag;//后面的，现在用不到

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, FGameplayTag>MagicAbilityWidgetTag;//后面的才会用，现在用不到
};
