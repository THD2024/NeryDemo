// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/NeryPlayerController.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include"Character/NeryCharacter.h"
#include "GameFramework/Character.h"
#include"Interface/CombatInterface.h"
#include"InputMappingContext.h"
#include "TimerManager.h"
#include"InputAction.h"

ANeryPlayerController::ANeryPlayerController()
{
	bReplicates = true;
}

void ANeryPlayerController::StartDetectiveTimer()
{
	//边缘情况检测逻辑
	if (!IsTargetValid(CurrentActor))//判断在选中过程中，敌人是否失效
	{
		UnLock(CurrentActor);
		LastActor = CurrentActor = nullptr;
		OnSelectedChanged.Execute();
	} 
}

void ANeryPlayerController::ClearDetectiveTimer()
{
	//清除当前timer
	if (GetWorldTimerManager().TimerExists(DetectiveTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DetectiveTimerHandle);
	}
}

void ANeryPlayerController::AutoStartorClearTimer()
{
	if (!CurrentActor)
	{	//表示当前没有锁定目标，则需要清空之前的启动的计时器
		ClearDetectiveTimer();
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Cyan, TEXT("是不是忘记死亡情况没有设置或者当前处于未锁定状态"));
		return;
	}
	//表示当前锁定了目标，启动计时器
	GetWorld()->GetTimerManager().SetTimer
	(
		DetectiveTimerHandle,
		this,
		&ANeryPlayerController::StartDetectiveTimer,
		DetectiveFrequency,
		true
	);
	
}

void ANeryPlayerController::UnLock(AActor* InActor)
{
	if (InActor && InActor->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_UnLockTargetFeedBack(InActor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Cyan, TEXT("检查接口实现"));
	}
}

void ANeryPlayerController::Lock(AActor* InActor)
{
	if (InActor && InActor->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_LockTargetFeedBack(InActor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Cyan, TEXT("检查接口实现"));
	}
}

bool ANeryPlayerController::IsTargetValid(AActor* InActor) const
{
	if (!InActor || !GetPawn())return false;
	//检查有没有超出范围
	if (GetPawn()->GetDistanceTo(InActor) > MaxDetectiveDistance)return false;
	FCollisionQueryParams LineQuery;
	LineQuery.AddIgnoredActor(GetPawn());
	FHitResult DetectiveResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel
	(
		DetectiveResult,
		GetPawn()->GetActorLocation(),
		InActor->GetActorLocation(),
		ECC_Visibility,
		LineQuery
	);
	//没有阻隔或者最终碰撞目标是需要的target
	return (!bHit || DetectiveResult.GetActor() == InActor);

}

void ANeryPlayerController::FindTargetInRadiusByObjectType(TArray<FOverlapResult>& OverlapResult, ECollisionChannel ObjectType, const float& Radius)
{
	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ObjectType);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetPawn());

	GetWorld()->OverlapMultiByObjectType
	(
		OverlapResult,
		GetPawn()->GetActorLocation(),
		FQuat::Identity,
		CollisionParams,
		CollisionShape,
		QueryParams
	);

}

void ANeryPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
	if (IsLocalController()) //确保只在本地控制器上注册输入映射上下文
	{
		check(DefaultMappingContext);

		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (!SubSystem)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SubSystem is null!"));

		}
		SubSystem->RemoveMappingContext(DefaultMappingContext);
		SubSystem->AddMappingContext(DefaultMappingContext, 0);
	}
	OnSelectedChanged.BindUObject(this,&ANeryPlayerController::AutoStartorClearTimer);
}

void ANeryPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerCameraManager)
	{
		//设置摄像机的俯仰角度范围
		PlayerCameraManager->ViewPitchMax = 30.f;
		PlayerCameraManager->ViewPitchMin = -60.f;
	}

	//注册输入映射上下文,添加到本地输入子系统
	
}

void ANeryPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	//绑定输入动作
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ANeryPlayerController::Jump);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANeryPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANeryPlayerController::Look);
		EnhancedInputComponent->BindAction(ShiftAction,ETriggerEvent::Triggered, this, &ANeryPlayerController::Shift_Hold);
		EnhancedInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed, this, &ANeryPlayerController::Shift_Release);
		//EnhancedInputComponent->BindAction(Crouch,ETriggerEvent::Triggered, this, &ANeryPlayerController::Crouch_Hold);
		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Started, this, &ANeryPlayerController::LockTarget);
	}

}

void ANeryPlayerController::Move(const FInputActionValue& Value)
{
	//获取当前实际的输入值
	const FVector2D MoveVector = Value.Get<FVector2D>();
	//获取到当前控制器的旋转值,只保留Yaw旋转
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	APawn* NeryCharacter = GetPawn<APawn>();
	if (!NeryCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("NeryCharacter is null!"));
		return;
	}
	//世界空间的前向和右向量
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	NeryCharacter->AddMovementInput(ForwardDirection, MoveVector.Y);
	NeryCharacter->AddMovementInput(RightDirection, MoveVector.X);
}

void ANeryPlayerController::Look(const FInputActionValue & Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
	
}

void ANeryPlayerController::Jump()
{
	if (APawn* MyPawn = GetPawn())
	{
		// 实际上是调用 ACharacter::Jump
		if (ACharacter* MyChar = Cast<ACharacter>(MyPawn))
		{
			MyChar->Jump();
		}
	}
}

void ANeryPlayerController::Shift_Release()
{
	if (ANeryCharacter* NeryChar = Cast<ANeryCharacter>(GetPawn()))
	{
		NeryChar->SetMaxWalkSpeed(NeryChar->RunNormalWalkSpeed);
	}
}

void ANeryPlayerController::LockTarget()
{
	//执行选中逻辑
	if (!GetPawn())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pawn is null!"));
		return;
	}
	TArray<FOverlapResult> OverlapResult;
	//查找范围内的敌人
	FindTargetInRadiusByObjectType(OverlapResult, ECC_Pawn, MaxDetectiveDistance);
	TArray<AActor*> ValidTarget;
	if (OverlapResult.Num() > 0)
	{
		for (int32 i = 0; i < OverlapResult.Num(); ++i)
		{	
			if (IsTargetValid(OverlapResult[i].GetActor()))
			{
				//目标有效的话,就添加到有效数组中去
				ValidTarget.AddUnique(OverlapResult[i].GetActor());
			}
		}
		if (ValidTarget.Num() > 0) //排除数组是空的情况
		{
			ValidTarget.RemoveSwap(LastActor);//防止选中和上一次一样的，提前在数组中将上一个查找删除
			//要么数组中啥都没有要么数组中有其他不是lastactor的validtarget
			if (ValidTarget.Num() > 0) 
			{
				CurrentActor = ValidTarget[FMath::RandRange(0, ValidTarget.Num() - 1)];
				UnLock(LastActor);
				LastActor = CurrentActor;
				Lock(CurrentActor);
				OnSelectedChanged.Execute();
			}
			else 
			{
				UnLock(LastActor);
				LastActor = CurrentActor = nullptr;
				OnSelectedChanged.Execute();
				ClearDetectiveTimer();//在当前没有锁定后在显示调用清除定时器函数保证定时器及时停止
			}
		}
	}
	else
	{
		UnLock(LastActor);
		LastActor = CurrentActor = nullptr;
		OnSelectedChanged.Execute();
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, TEXT("没有可以被锁定的敌人"));
	}
}


//void ANeryPlayerController::Crouch_Hold()
//{
//	if (ACharacter* MyChar = GetPawn<ACharacter>())
//	{
//		MyChar->Crouch();
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Crouch"));
//
//	}
//}

void ANeryPlayerController::Shift_Hold()
{
	if (ANeryCharacter* NeryChar = Cast<ANeryCharacter>(GetPawn()))
	{
		NeryChar->SetMaxWalkSpeed(NeryChar->RunMaxWalkSpeed);
	}
}
