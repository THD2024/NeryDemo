// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include"GameplayTagContainer.h"
#include "NeryGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API UNeryGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag InputTag;
};
