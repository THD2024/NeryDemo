// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/NeryGameplayAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include"Interface/CombatInterface.h"

void UNeryGameplayAbility::MotionWarping(AActor* SelfActor, AActor* TargetActor, FName NotifyName)
{
	if (!SelfActor || !TargetActor)return;
	FVector SelfLocation = SelfActor->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector Direction = (SelfLocation - TargetLocation).GetSafeNormal2D();//获取到目标指向自己的单位向量，没有z，用来后面计算攻击最后停在目标前面什么位置
	float FinalRadius = 0.f;
	float SelfRadius = 0.f;
	float TargetRadius = 0.f;
	float ActualDistance = FVector::Dist(SelfLocation, TargetLocation);
	if (ACharacter* Self = Cast<ACharacter>(SelfActor))
	{
		 SelfRadius = Self->GetCapsuleComponent()->GetScaledCapsuleRadius();
	}
	if (ACharacter* Target = Cast<ACharacter>(TargetActor))
	{
		 TargetRadius = Target->GetCapsuleComponent()->GetScaledCapsuleRadius();	
	}
	//(WarpDistance = 目标胶囊体半径 + 攻击者胶囊体半径 + 武器有效攻击距离\)
	FinalRadius = SelfRadius + TargetRadius + 10.f;
	//最终位置：目标位置 + 到理想距离*朝向到自己的方向
	FVector FinalWarpLocation;
	if (ActualDistance < FinalRadius)
	{
		FinalWarpLocation = SelfLocation;
	}
	else
	{
		FinalWarpLocation = TargetLocation + Direction * FinalRadius;
	}
	FinalWarpLocation.Z = SelfLocation.Z;//防止目标在楼梯上，然后z轴就会高一些，也就是停的目标位置可以是在半空中，所以需要将z轴和自己原来的值设置一致
	
	FRotator FinalWarpRotation = (-Direction).Rotation();//设置自己朝向方向刚好是目标方向
	if (SelfActor->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_UpdateWarpTarget(SelfActor,NotifyName, FinalWarpLocation, FinalWarpRotation);
	}
}

void UNeryGameplayAbility::PlayAnimationSequence(UAnimSequenceBase* Sequence, float PlayRate)
{
	ClearAnimTimer();
	if (!Sequence){OnAnimSequenceCancelled();}
	
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar){OnAnimSequenceCancelled();}
	
	
	USkeletalMeshComponent* Mesh = Avatar->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh){OnAnimSequenceCancelled();}
	
	OriginalAnim = Mesh->GetAnimInstance()->GetClass();
	if (!OriginalAnim){OnAnimSequenceCancelled();}
	
	ActiveSequence = Sequence;
	
	Mesh->PlayAnimation(Sequence, false);
	NetMulticast_PlayAnim(Mesh, Sequence);
	
	if (UAnimInstance* NewAnimInstance = Mesh->GetAnimInstance())
	{
		NewAnimInstance->RootMotionMode = ERootMotionMode::RootMotionFromEverything;

	}
	float Duration = Sequence->GetPlayLength() / FMath::Max(PlayRate, SMALL_NUMBER);
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindWeakLambda(this, [this]()
	{//动画播放完后触发这个时间句柄回调，来进行清理
		ActiveSequence = nullptr;
		RestoreAnimBp();
		OnAnimSequenceFinished();
	});

	Avatar->GetWorldTimerManager().SetTimer(//启动时间句柄
		AnimTimerHandle,
		TimerDelegate,
		Duration,
		false);
}


void UNeryGameplayAbility::NetMulticast_PlayAnim_Implementation(USkeletalMeshComponent* Mesh,
	UAnimSequenceBase* AnimSequenceBase)
{
	if (Mesh && AnimSequenceBase)
	{
		Mesh->PlayAnimation(AnimSequenceBase, false);
	}
}

void UNeryGameplayAbility::NetMulticast_RestoreAnimBp_Implementation()
{
	if (!OriginalAnim) return;

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;

	USkeletalMeshComponent* Mesh = Avatar->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh) return;

	// 归还控制权给原始 AnimBP
	Mesh->SetAnimInstanceClass(OriginalAnim);

	// 重建 AnimInstance 后立即 Tick 一次，让状态机立刻响应
	Mesh->InitAnim(true);
}

void UNeryGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActiveSequence)
	{
		ClearAnimTimer();
		ActiveSequence = nullptr;
		RestoreAnimBp();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}

void UNeryGameplayAbility::RestoreAnimBp()
{
	if (!OriginalAnim) return;

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar) return;

	USkeletalMeshComponent* Mesh = Avatar->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh) return;

	// 归还控制权给原始 AnimBP
	Mesh->SetAnimInstanceClass(OriginalAnim);

	// 重建 AnimInstance 后立即 Tick 一次，让状态机立刻响应
	Mesh->InitAnim(true);
	NetMulticast_RestoreAnimBp();
}

void UNeryGameplayAbility::ClearAnimTimer()
{
	if (AnimTimerHandle.IsValid())
	{
		if (AActor* Avatar = GetAvatarActorFromActorInfo())
		{
			Avatar->GetWorldTimerManager().ClearTimer(AnimTimerHandle);
		}
	}
}




