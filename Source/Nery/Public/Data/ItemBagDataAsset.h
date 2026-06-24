// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"GameplayTagContainer.h"
#include "ItemBagDataAsset.generated.h"


class UGameplayEffect;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FItemInformation
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText BaseInformation = FText();

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag BuffTag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> BuffEffect;

	UPROPERTY(VisibleAnywhere)
	int32 StorageNumber = 0;
};

UCLASS(BlueprintType,Blueprintable)
class NERY_API UItemBagDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FItemInformation> ItemBag;

	void AddItem(const FGameplayTag& InTag);

	void ReduceItem(const FGameplayTag& InTag);

	TSubclassOf<UGameplayEffect> FindSpecificEffectByTag(const FGameplayTag& InTag);

	FItemInformation& GetSpecificItemInfoByTag(const FGameplayTag& InTag);
};
