// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include"GameplayTagContainer.h"
#include "CombatInterface.generated.h"

class UItemBagDataAsset;
// This class does not need to be modified.

UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NERY_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void LockTargetFeedBack();

	UFUNCTION(BlueprintNativeEvent)
	void UnLockTargetFeedBack();

	UFUNCTION(BlueprintNativeEvent)
	ECharacterAttackState GetAttackState();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateAttributeMenu(bool CanOpen);

	UFUNCTION(BlueprintNativeEvent)
	void CallAddBuffNumber(const FGameplayTag& InTag,AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent)
	UItemBagDataAsset* GetItemBag();

	UFUNCTION(BlueprintNativeEvent)
	TMap<FGameplayTag,int32> GetBuffNumber();
};
