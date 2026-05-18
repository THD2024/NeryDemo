// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/NeryPlayerController.h"
#include"EnhancedInputSubsystems.h"
#include"EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include"InputMappingContext.h"
#include"InputAction.h"

ANeryPlayerController::ANeryPlayerController()
{}

void ANeryPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (PlayerCameraManager)
	{
		//设置摄像机的俯仰角度范围
		PlayerCameraManager->ViewPitchMax = 30.f;
		PlayerCameraManager->ViewPitchMin = -60.f;
	}

	check(DefaultMappingContext);
	//注册输入映射上下文,添加到本地输入子系统
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!SubSystem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SubSystem is null!"));

	}
	SubSystem->AddMappingContext(DefaultMappingContext,0);
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

	}

}

void ANeryPlayerController::Move(const FInputActionValue& Value)
{
	if (!GetPawn())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GetPawn is null!"));
		return;
	}

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
	if (!GetPawn())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GetPawn is null!"));
		return;
	}

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
