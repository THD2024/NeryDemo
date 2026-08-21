// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/Interface.h"
#include"GameplayTagContainer.h"
#include "CombatInterface.generated.h"

class UItemBagDataAsset;
class UAbilityWidgetController;
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

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void CallUpgradeAttribute(const FGameplayTag& AttributeTag);
	
	UFUNCTION(BlueprintNativeEvent)
	void AllowAttack();

	UFUNCTION(BlueprintNativeEvent)
	void CloseAttack();
	
	UFUNCTION(BlueprintNativeEvent)
	void BegintoTrace();
	
	UFUNCTION(BlueprintNativeEvent)
	TArray<FGameplayTag> GetCharacterActivateAbilities();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FTransform GetWeaponLocation();
	
	UFUNCTION(BlueprintNativeEvent)
	void UpdateAbiltyMenu(bool InCanOpenAbilityMenu);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void UpdateAbilityWidgetSlot(const FGameplayTag& InInputTag, const FGameplayTag& InTag);

	UFUNCTION(BlueprintNativeEvent)
	void OnAbilityWidgetControllerSet(UAbilityWidgetController* AbilityWidgetController);
	
	UFUNCTION(BlueprintNativeEvent)
	void UpdateWarpTarget(FName TargetName, FVector TargetLocation, FRotator TargetRotation);
	
	UFUNCTION(BlueprintNativeEvent)
	bool GetEnemyPoiseStatus();
	
	UFUNCTION(BlueprintNativeEvent)
	void ActiveHitReaction();//敌人和玩家都继承这个，只需要在这里面调用技能激活函数，不过需要的是首先将受击技能都添加到敌人和玩家拥有的技能池中.

	UFUNCTION(BlueprintNativeEvent)
	void SendEventtoHitReaction(const FGameplayTag& InTag,const FGameplayEventData& EventData);
};
