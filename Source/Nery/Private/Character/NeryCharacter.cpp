// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryCharacter.h"
#include"GameFramework/CharacterMovementComponent.h"

ANeryCharacter::ANeryCharacter()
{
	//防止角色跟随控制器旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //角色移动时旋转朝向
}

void ANeryCharacter::BeginPlay()
{
	Super::BeginPlay();
}
