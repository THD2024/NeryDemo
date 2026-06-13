// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"GameplayTagsManager.h"
#include "AttributeInfo.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FNeryAttributeInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FText AttributeName = FText();
};

UCLASS(Blueprintable,BlueprintType)
class NERY_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	const FNeryAttributeInfo GetAttributeInfoByTag(const FGameplayTag& GameplayTag, bool bIsNotFound = true);

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FNeryAttributeInfo> AttributeInfos;


};
