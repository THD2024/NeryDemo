// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryCharacter.h"
#include"PlayerState/NeryPlayerState.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"UI/Controller/WidgetController.h"
#include"UI/HUD/NeryHUD.h"

ANeryCharacter::ANeryCharacter()
{
	//防止角色跟随控制器旋转
	bReplicates = true;
	SetReplicateMovement(true);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; //角色移动时旋转朝向
}

void ANeryCharacter::InitHUD()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		APlayerState* PS = GetPlayerState();
		check(PS);
		FWidgetControllerParams Params(PS,PC,AbilitySystemComponent,AttributeSet);
		if(ANeryHUD* HUD = Cast<ANeryHUD>(PC->GetHUD()))
		{	
			HUD->InitWidgetAndController(Params);
		}
	}
}

void ANeryCharacter::SetMaxWalkSpeed(float NewMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
	if(!HasAuthority())
	{//判断当前是客户端，就通知服务器我的速度发生了变化，服务器来设置角色的移动速度
		Server_SetMaxWalkSpeed(NewMaxWalkSpeed);
	}
}

void ANeryCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANeryCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitASCandAttribute();
	if (IsLocallyControlled())
	{
		InitHUD();
	}
}

void ANeryCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("OnRep_PlayerState called in Character"));
	InitASCandAttribute();
	InitHUD();//Hud属于表现层，在这里调用初始化Hud的函数，来确保在玩家状态复制到客户端后，客户端的Hud能够正确显示玩家状态的信息。
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

void ANeryCharacter::Server_SetMaxWalkSpeed_Implementation(float NewMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
}
