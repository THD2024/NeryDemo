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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//暴露给蓝图的变量
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Combat")
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Combat")
	float MovementRotation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsLockOn;

private:
	UPROPERTY()
	class ANeryCharacter* OwingCharacter;
};
