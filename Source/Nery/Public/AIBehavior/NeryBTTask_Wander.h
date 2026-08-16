// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NeryBTTask_Wander.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class NERY_API UNeryBTTask_Wander : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UNeryBTTask_Wander();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
protected:
	UPROPERTY(EditAnywhere,Category = "AI")
	FBlackboardKeySelector TargetLocationSelector;
	
};
