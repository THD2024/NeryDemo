// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/NeryPlayerController.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include"Character/NeryCharacter.h"
#include "GameFramework/Character.h"
#include"Interface/CombatInterface.h"
#include"InputMappingContext.h"
#include"InputAction.h"

ANeryPlayerController::ANeryPlayerController()
{
	bReplicates = true;
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
	}
	TArray<FOverlapResult> OverlapResult;
	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(500.f);
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

	TArray<AActor*> EnableLockActors;
	if (OverlapResult.Num() > 0)
	{
		FVector TraceStart = GetPawn()->GetActorLocation();
		FCollisionQueryParams LineQuery;
		LineQuery.AddIgnoredActor(GetPawn());
		for (int32 i = 0; i < OverlapResult.Num(); ++i)
		{	
			AActor* TargetActor = OverlapResult[i].GetActor();
			FVector TraceEnd = TargetActor->GetActorLocation();
			//筛选出没有被阻隔的敌人
			FHitResult HitResult;

			bool bHit = GetWorld()->LineTraceSingleByChannel
			(
				HitResult,
				TraceStart,
				TraceEnd,
				ECC_Visibility,
				LineQuery
			);
			if (!bHit || HitResult.GetActor() == TargetActor)
			{
				//没有发生阻挡
				EnableLockActors.AddUnique(OverlapResult[i].GetActor());
			}
		}
		if (EnableLockActors.Num() == 1 && EnableLockActors[0] == LastActor)
		{//当敌人只有一个时
			if (LastActor && LastActor->Implements<UCombatInterface>())
			{ 
				ICombatInterface::Execute_UnLockTargetFeedBack(LastActor);
				LastActor = nullptr;
				return;
			}
		}
		if (LastActor && LastActor->Implements<UCombatInterface>())
		{  //先将上一个取消锁定
			ICombatInterface::Execute_UnLockTargetFeedBack(LastActor);
		}
		if (EnableLockActors.Num() > 0) 
		{
			EnableLockActors.RemoveSwap(LastActor);//防止选中和上一次一样的，提前在数组中将上一个查找删除
		}
		if (EnableLockActors.Num() > 0)
		{
			CurrentActor = EnableLockActors[FMath::RandRange(0, EnableLockActors.Num() - 1)];
			LastActor = CurrentActor;
			if (CurrentActor && CurrentActor->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_LockTargetFeedBack(CurrentActor);
			}
		}
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, TEXT("没有可以被锁定的敌人"));
		if (LastActor&&LastActor->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_UnLockTargetFeedBack(LastActor);
			LastActor = nullptr;
			CurrentActor = nullptr;
		}
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
