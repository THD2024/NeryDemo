// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"ScalableFloat.h"
#include"GameplayTagContainer.h"
#include"GameplayEffect.h"
#include "LevelUpInfo.generated.h"

/**
 * 
 */
//该数据资产的功能仅用于neryattribute升级时进行数据参考和基础等级伤害提供

USTRUCT(BlueprintType)
struct FLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat XPToLevel;//经验值到下一级所需的经验值

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat AttributePointsPerLevel;//每级奖励的属性点数

	UPROPERTY(EditDefaultsOnly)
	FScalableFloat Damage_Normal;//普通伤害

};

UCLASS(Blueprintable,BlueprintType)
class NERY_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//这里需要注意，Int32类型只在最后显示的时候返回是int32，除此之外，所有的计算都是float类型，避免了int32类型在计算中出现的精度问题
	float GetXPFromLevel(float Level) const
	{
		return LevelUpInfo.XPToLevel.GetValueAtLevel(Level);//y = f(x)，所以这里的x是经验值，y是等级
	}

	float GetAttributePointsFromLevel(float Level) const
	{
		return LevelUpInfo.AttributePointsPerLevel.GetValueAtLevel(Level);
	}
	float GetNormalDamageFromLevel(float Level) const
	{
		return LevelUpInfo.Damage_Normal.GetValueAtLevel(Level);
	}

	TSubclassOf<UGameplayEffect> GetUpgradeBasicAttributeEffect()
	{
		if (UpgradeBasicAttributeEffect)
		{
			return UpgradeBasicAttributeEffect;
		}
		return TSubclassOf<UGameplayEffect>();
	}

	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelUp")
	FLevelInfo LevelUpInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelUp")
	TSubclassOf<UGameplayEffect> UpgradeBasicAttributeEffect;//这个是专门的用来实现技能加点的游戏效果
	
};
