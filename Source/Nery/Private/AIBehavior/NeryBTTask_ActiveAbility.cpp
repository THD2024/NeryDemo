// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBehavior/NeryBTTask_ActiveAbility.h"
#include "AIController.h"
#include"Character/EnemyCharacter.h"
#include"AbilitySystemComponent.h"
#include"AbilitySystem/NeryGameplayTag.h"
#include "BehaviorTree/BlackboardComponent.h"
#include"AbilitySystemBlueprintLibrary.h"


UNeryBTTask_ActiveAbility::UNeryBTTask_ActiveAbility()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UNeryBTTask_ActiveAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!AIController || !EnemyCharacter)return EBTNodeResult::Failed;
	
	ASC = EnemyCharacter->AbilitySystemComponent;
	if (!ASC)return EBTNodeResult::Failed;
	
	FDelegateHandle Handle = ASC->OnAbilityEnded.AddUObject(this, &UNeryBTTask_ActiveAbility::OnAbilityEnded,&OwnerComp);
	bool bActive = EnemyCharacter->ActivateEnemyAbilityByTag(AbilityTag);
	if (!bActive)
	{
		ASC->OnAbilityEnded.Remove(Handle);
		return EBTNodeResult::Failed;//没有成功激活就直接返回失败
	}
	
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (Blackboard)
	{//作为一个通用的事件发送到需要接收的地方
		FGameplayEventData Payload;
		AActor* Actor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActor.SelectedKeyName));
		Payload.Instigator = Actor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EnemyCharacter,FNeryGameplayTags::GetNeryGameplayTags().Event_TargetInfo,Payload);
	}
		
	
	return EBTNodeResult::InProgress;//反之就是成功了，正在释放技能中
}

void UNeryBTTask_ActiveAbility::OnAbilityEnded(const FAbilityEndedData& AbilityEndedData,
	UBehaviorTreeComponent* OwnerComp)
{
	if (AbilityEndedData.AbilityThatEnded && AbilityEndedData.AbilityThatEnded->AbilityTags.HasTagExact(AbilityTag))
	{
		//进来了就代表能力结束了，那么就可以移除委托并且返回成功
		if (ASC)
		{
			ASC->OnAbilityEnded.RemoveAll(this);
			FinishLatentTask(*OwnerComp,EBTNodeResult::Succeeded);//手动结束
		}
	}
	
}
