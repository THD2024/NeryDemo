// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/AttackAnimNotify.h"
#include"Interface/CombatInterface.h"

void UAttackAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (MeshComp->GetOwner() != nullptr)
	{
		if (MeshComp->GetOwner()->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_AllowAttack(MeshComp->GetOwner());
		}
	}
}

void UAttackAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (MeshComp->GetOwner() != nullptr)
	{
		if (MeshComp->GetOwner()->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_CloseAttack(MeshComp->GetOwner());
		}
	}
}

// void UAttackAnimNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
// 	const FAnimNotifyEventReference& EventReference)
// {
// 	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
// 	if (MeshComp->GetOwner() != nullptr)
// 	{
// 		if (MeshComp->GetOwner()->Implements<UCombatInterface>())
// 		{
// 			ICombatInterface::Execute_BegintoTrace(MeshComp->GetOwner());
// 		}
// 	}
// }

