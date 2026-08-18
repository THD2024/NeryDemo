// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBehavior/NeryBTService_LockTarget.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include "AbilitySystem/NeryGameplayTag.h"
#include"AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UNeryBTService_LockTarget::UNeryBTService_LockTarget()
{
	NodeName = "AutoLockActor";
	bNotifyTick = true;
	Interval = 0.05f;
}

void UNeryBTService_LockTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (!OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->GetPawn())return;
	AAIController* AIController = OwnerComp.GetAIOwner();
	ACharacter* OwnerActor =Cast<ACharacter>(AIController->GetPawn());
	UCharacterMovementComponent* CMC = OwnerActor->GetCharacterMovement();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKeySelector.SelectedKeyName));
	if (!CMC || !TargetActor)return;
	
	if (LockTag.MatchesTagExact(FNeryGameplayTags::GetNeryGameplayTags().Status_Enemy_UnLocked))
	{
		OwnerActor->bUseControllerRotationYaw = false;	
		CMC->bOrientRotationToMovement = true;
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerActor))
		{
			EnemyCharacter->SetIsLocked(false);
		}
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	if (LockTag.MatchesTagExact(FNeryGameplayTags::GetNeryGameplayTags().Status_Enemy_Locked))
	{
		OwnerActor->bUseControllerRotationYaw = true;
		CMC->bOrientRotationToMovement = false;
		if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwnerActor))
		{
			EnemyCharacter->SetIsLocked(true);
		}
		// UpdateRotation(OwnerActor,TargetActor,AIController,DeltaSeconds);
		AIController->SetFocus(TargetActor);
	}
	
}

void UNeryBTService_LockTarget::UpdateRotation(AActor* OwnerActor,AActor* TargetActor,AAIController* AIController,float DeltaSeconds)
{
	if (!TargetActor || !OwnerActor) return;
	
	FVector OwnerLocation = OwnerActor->GetActorLocation();
	OwnerLocation.Z +=50.f;//避免获取到脚的位置
	FVector TargetLocation = TargetActor->GetActorLocation();
	TargetLocation.Z +=50.f;
	
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(OwnerLocation,TargetLocation);
	TargetRotation.Pitch -= 15.0f;
	
	//通过cha值转向
	FRotator CurrentRotation = OwnerActor->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation,TargetRotation,DeltaSeconds,InterpSpeed);
	AIController->SetControlRotation(NewRotation);
	
}
