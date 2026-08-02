// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "WidgetSlotTagInfo.generated.h"
/**
 * 
 */

//重大问题，玩家个人的技能分配和增益道具怎么能够放到gamestate作为全局单例存在。
//不过因为时间问题，增益道具全服就全服吧，但是这里关于前面写的ability的代码全部报废。
DECLARE_MULTICAST_DELEGATE(FOnAbilityWidgetChangeDelegate);

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

USTRUCT(BlueprintType)
struct FAbilityWidgetSlotTag
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;//表示当前按的是哪个插槽
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TypeTag;//用来过滤是不是对应的类型的技能比如物理和法术
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AbilitySlotTag;//对应插槽内部显示的技能,这里ui和内部能力逻辑分开，直接通过获取到技能然后就可以激活
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
	
	void SetMagicAbilitySlotTag(const FGameplayTag& AbilitySlotTag);//因为只有一个法术插槽，所以这里不需要通过inputtag来区分，如果后面需要扩展，参考物理插槽
	
	void SetPhysicalAbilitySlotTag(const FGameplayTag& InInputTag,const FGameplayTag& AbilitySlotTag);
	
	FGameplayTag GetAbilitySlotTagByInputTag(const FGameplayTag& InInputTag);//这里是法术和物理通用的，都是通过唯一的inputtag来区分插槽
	
	FOnAbilityWidgetChangeDelegate OnAbilitySlotTagChanged;
	
protected:
	//这三个的key在蓝图中进行填充。因为是固定的，只有value是变化的，通过代码实现变化
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Replicated)
	FWidgetSlotTag BuffSlotTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
	TArray<FAbilityWidgetSlotTag>  PhysicalAbilitySlotTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
	FAbilityWidgetSlotTag MagicAbilitySlotTags;//这个只有一个
	
};
