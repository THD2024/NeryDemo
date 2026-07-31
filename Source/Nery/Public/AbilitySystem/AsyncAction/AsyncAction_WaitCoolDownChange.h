// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_WaitCoolDownChange.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWaitCoolDownDelegate,float ,DurationTime,float,RemainingTime);

UCLASS(BlueprintType,meta = (ExposedAsyncProxy = "AsyncAction")) 
class NERY_API UAsyncAction_WaitCoolDownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",Category = "AsnycAction"), meta = (DefaultToSelf = "Target"))
	static UAsyncAction_WaitCoolDownChange* WaitCoolDownChange( UAbilitySystemComponent* InASC, const FGameplayTag& InCoolDownTag);
	
	virtual void Activate() override;
	
	virtual void SetReadyToDestroy() override;

	UFUNCTION(BlueprintCallable)
	void EndTask();
	

	void OnTagChanged(const FGameplayTag InTag, int32 NewCount);
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>ASC;
	
	UPROPERTY()
	FGameplayTag CoolDownTag;
	
	UPROPERTY(BlueprintAssignable)
	FWaitCoolDownDelegate CoolDownDelegate;
};
