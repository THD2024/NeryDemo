// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NeryBTTask_ActiveAbility.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class NERY_API UNeryBTTask_ActiveAbility : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UNeryBTTask_ActiveAbility();
	
	UPROPERTY(EditAnywhere,Category = "AI")
	FGameplayTag AbilityTag;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UFUNCTION()
	void OnAbilityEnded(const FAbilityEndedData& AbilityEndedData, UBehaviorTreeComponent* OwnerComp);	
	
private:
	UPROPERTY()
	class UAbilitySystemComponent* ASC;
};
