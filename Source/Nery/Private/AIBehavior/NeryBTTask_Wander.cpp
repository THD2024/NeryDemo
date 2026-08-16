// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBehavior/NeryBTTask_Wander.h"
#include"AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UNeryBTTask_Wander::UNeryBTTask_Wander()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UNeryBTTask_Wander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner()->GetPawn())return EBTNodeResult::Failed;
	APawn* SelfActor = OwnerComp.GetAIOwner()->GetPawn();
	
	FVector SelfLocation = SelfActor->GetActorLocation();
	FVector SelfRightLocation = SelfActor->GetActorRightVector();//右边单位向量
	float Direction = FMath::RandBool() ? 1.0f : -1.0f;//随机决定左右
	float Distance = FMath::RandRange(100.f,300.f);
	
	FVector TargetLocation = SelfLocation + (SelfRightLocation * Direction * Distance);
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSys)return EBTNodeResult::Failed;
	FNavLocation ExampleLocation;
	if (!NavSys->ProjectPointToNavigation(TargetLocation, ExampleLocation,FVector(150,150,150)))
	{
		 NavSys->GetRandomReachablePointInRadius(SelfLocation,150,ExampleLocation);
		TargetLocation = ExampleLocation.Location;
	}
	
	if (!OwnerComp.GetBlackboardComponent())return EBTNodeResult::Failed;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationSelector.SelectedKeyName,TargetLocation);
	return EBTNodeResult::Succeeded;
}
