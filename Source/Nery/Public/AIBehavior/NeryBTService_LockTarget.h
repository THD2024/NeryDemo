// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "NeryBTService_LockTarget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class NERY_API UNeryBTService_LockTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UNeryBTService_LockTarget();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	void UpdateRotation(AActor* OwnerActor,AActor* TargetActor,AAIController* AIController,float DeltaSeconds);
	
protected:
	UPROPERTY(EditAnywhere,Category = "AI")
	FBlackboardKeySelector TargetKeySelector;
	
	UPROPERTY(EditAnywhere,Category = "AI")
	FGameplayTag LockTag;
	
	UPROPERTY(EditAnywhere,Category = "AI")
	float InterpSpeed;
};
