// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryCharacter.h"
#include"PlayerState/NeryPlayerState.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"GameFramework/CharacterMovementComponent.h"

ANeryCharacter::ANeryCharacter()
{
	//防止角色跟随控制器旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; //角色移动时旋转朝向
}

void ANeryCharacter::SetMaxWalkSpeed(float NewMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
}

void ANeryCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANeryCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitASCandAttribute();
	
}

void ANeryCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitASCandAttribute();
}

void ANeryCharacter::InitASCandAttribute()
{
	ANeryPlayerState* PS = Cast<ANeryPlayerState>(GetPlayerState());
	if (PS)
	{
		AbilitySystemComponent = PS->AbilitySystemComponent;
		AttributeSet = PS->AttributeSet;
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
		}
	}
}
