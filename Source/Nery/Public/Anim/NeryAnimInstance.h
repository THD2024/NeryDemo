// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NeryAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API UNeryAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//线程安全的动画更新函数，适用于多线程动画更新的情况,代替NativeUpdateAnimation函数
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	//暴露给蓝图的变量
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Combat")
	float MovementSpeed = 0.f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Combat")
	float MovementRotation = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsLockOn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bFalling = false;

};
