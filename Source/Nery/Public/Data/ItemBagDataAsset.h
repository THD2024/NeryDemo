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
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText BaseInformation = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag BuffTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> BuffEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ActorName = FText();

	UPROPERTY(BlueprintReadOnly)
	int32 StorageNumber = 0;//这个不能手动更改，这里保留用来后面传递信息到widget中。
};

UCLASS(BlueprintType,Blueprintable)
class NERY_API UItemBagDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FItemInformation> ItemBag;

	TSubclassOf<UGameplayEffect> FindSpecificEffectByTag(const FGameplayTag& InTag);

	FItemInformation GetSpecificItemInfoByTag(const FGameplayTag& InTag);
};
