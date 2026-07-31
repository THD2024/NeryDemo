// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"AbilitySystem/GameplayAbility/NeryGameplayAbility.h"
#include "Engine/DataAsset.h"
#include"GameplayTagContainer.h"
#include "NeryAbilityDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNeryAbilityInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UNeryGameplayAbility> AbilityClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityTag;//这里的abilitytag，同时需要在赋予能力的时候添加到dynamictag，摆脱inputtag的束缚
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AbilityMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> AbilityIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AbilityName = FName("");
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AbilityDescription = FText();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityCoolDownTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityCostsTag;
};

UCLASS(Blueprintable,BlueprintType)
class NERY_API UNeryAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FNeryAbilityInfo> AbilityInfos;
	
	UFUNCTION(BlueprintCallable)
	FNeryAbilityInfo GetAbilityInfoByTag(const FGameplayTag& AbilityTag) const;
	
	TSubclassOf<UNeryGameplayAbility> GetAbilityClassByTag(const FGameplayTag& AbilityTag) const;
};
