// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include"GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterDataAsset.generated.h"

/**
 * 
 */


UCLASS(Blueprintable,BlueprintType)
class NERY_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<UAnimMontage*> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UGameplayEffect> AttackEffectClass;//角色的普通伤害

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UGameplayEffect> SecondaryAttributeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UGameplayEffect> VitalAttributeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TArray<TSubclassOf<UGameplayAbility>> BasicAbilitiesClass;
};
