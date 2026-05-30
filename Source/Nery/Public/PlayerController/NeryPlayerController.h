// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include"InputActionValue.h"
#include "NeryPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FTimerHandle;
DECLARE_DELEGATE(FOnSelectedChangeDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLinkAnimTiming, bool);
/**
 * 
 */
UCLASS()
class NERY_API ANeryPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ANeryPlayerController();
	bool IsLocked() { return IsValid(CurrentActor); }
	FOnLinkAnimTiming OnLinkAnimTiminig;
protected:
	AActor* LastActor = nullptr;
	AActor* CurrentActor = nullptr;
	FTimerHandle DetectiveTimerHandle;//用来执行检测目标被动取消选择的情况
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float MaxDetectiveDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DetectiveFrequency = 0.2f;

	FOnSelectedChangeDelegate OnSelectedChanged;
	void StartDetectiveTimer();
	void ClearDetectiveTimer();
	void AutoStartorClearTimer();
	void UnLock(AActor* InActor);//启动锁定特效
	void Lock(AActor* InActor);//取消锁定特效
	bool IsTargetValid(AActor* InActor) const ;//判断当前检测到的对象是否有效，是否超出距离，是否中间有阻挡

	//根据对象类型来检测一定范围内的目标,忽略目标自动设置为当前本身
	void FindTargetInRadiusByObjectType(TArray<FOverlapResult>& OverlapResult, ECollisionChannel ObjectType, const float& Radius);



	virtual void AcknowledgePossession(APawn* P) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//输入动作
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> Crouch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LockAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	//输入回调函数
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Shift_Hold();
	void Shift_Release();
	void LockTarget();

	//	void Crouch_Hold();
	


};
