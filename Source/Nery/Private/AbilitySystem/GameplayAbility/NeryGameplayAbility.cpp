// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbility/NeryGameplayAbility.h"
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
