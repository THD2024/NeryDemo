// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NeryAnimInstance.h"
#include"Character/NeryCharacter.h"
#include"GameFrameWork/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"


void UNeryAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	APawn* OwningPawn = TryGetPawnOwner();

	if (!OwningPawn)return;
	FVector Velocity = OwningPawn->GetVelocity();
	MovementSpeed = Velocity.Size2D();//只获取到水平面上的速度

	//计算8方向角度
	if (MovementSpeed > 2.0f)//只有移动的是否才会计算角度
	{
		MovementRotation = UKismetAnimationLibrary::CalculateDirection(
			Velocity, OwningPawn->GetActorRotation());
	}
	if(ANeryCharacter* NeryCharacter = Cast<ANeryCharacter>(OwningPawn))
	{
		bIsLockOn = NeryCharacter->bIsLockOn_NetWorked;
		bFalling = NeryCharacter->GetCharacterMovement()->IsFalling();
	}
}
