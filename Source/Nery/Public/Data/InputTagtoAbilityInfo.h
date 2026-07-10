// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"InputAction.h"
#include"GameplayTagContainer.h"
#include "InputTagtoAbilityInfo.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FInputTagAbilityInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag InputTag;
};

UCLASS(Blueprintable,BlueprintType)
class NERY_API UInputTagtoAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FInputTagAbilityInfo GetInputTagAbilityInfoByTag(const FGameplayTag& InputTag);//根据标签获取数组中对应的Info

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FInputTagAbilityInfo> InputTagtoAbility;
};
