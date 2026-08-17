// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NeryAnimInstance.h"
#include"Character/NeryCharacter.h"
#include"GameFrameWork/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Character/EnemyCharacter.h"


void UNeryAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	ACharacter* OwningCharacter = Cast<ACharacter>(TryGetPawnOwner());

	if (!OwningCharacter)return;
	FVector Velocity = OwningCharacter->GetVelocity();
	MovementSpeed = Velocity.Size2D();//只获取到水平面上的速度

	//计算8方向角度
	if (MovementSpeed > 2.0f)//只有移动的是否才会计算角度
	{
		MovementRotation = UKismetAnimationLibrary::CalculateDirection(
			Velocity, OwningCharacter->GetActorRotation());
	}
	if (OwningCharacter->GetMovementComponent())
	{
		bFalling = OwningCharacter->GetMovementComponent()->IsFalling();
	}
	if(ANeryCharacter* NeryCharacter = Cast<ANeryCharacter>(OwningCharacter))
	{
		//仅当当前动画的Owning是Player是才能判断当前是否锁定
		bIsLockOn = NeryCharacter->bIsLockOn_NetWorked;
	}
	if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(OwningCharacter))
	{
		bIsLockOn = EnemyCharacter->GetIsLocked();
	}
	
}
