// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NeryAnimInstance.h"
#include"Character/NeryCharacter.h"
#include"GameFrameWork/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UNeryAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwingCharacter)
	{
		OwingCharacter = Cast<ANeryCharacter>(TryGetPawnOwner());
	}

	if (!OwingCharacter)return;
	FVector Velocity = OwingCharacter->GetVelocity();
	MovementSpeed = Velocity.Size2D();//只获取到水平面上的速度

	//计算8方向角度
	if (MovementSpeed > 2.0f)//只有移动的是否才会计算角度
	{
		MovementRotation = UKismetAnimationLibrary::CalculateDirection(
			Velocity, OwingCharacter->GetActorRotation());
	}

	bIsLockOn = OwingCharacter->GetIsLockOn();
	
}
