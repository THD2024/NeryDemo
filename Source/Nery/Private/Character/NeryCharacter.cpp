// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryCharacter.h"
#include"PlayerState/NeryPlayerState.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"PlayerController/NeryPlayerController.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"UI/Controller/WidgetController.h"
#include"Data/CharacterDataAsset.h"
#include "Net/UnrealNetwork.h"
#include"EffectActor/Weapon.h"
#include"Components/ActorComponent.h"
#include"UI/HUD/NeryHUD.h"

ANeryCharacter::ANeryCharacter()
{
	//防止角色跟随控制器旋转	
	bReplicates = true;
	SetReplicateMovement(true);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = RunNormalWalkSpeed;
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
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(GetController()))
	{
		PC->OnLinkAnimTiminig.AddLambda([this](bool IsLockOn) {
			//LinkAnimTiming(IsLockOn);//判断当前的锁定状态，来通知character来Linkanim，来实现不同的动画表现
			SetLockMode(IsLockOn);//根据当前的锁定状态来设置锁定模式
			});
		
	}
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(GetController()))
	{
		//接收到攻击按键输入事件
		PC->OnAttackInput.AddUObject(this, &ANeryCharacter::ReceiveAttackInput);
	}
	SpawnWeapon();
}

void ANeryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled())
	{
		if (bIsLockOn_NetWorked && NetLockedTarget)
		{
			SmoothRotateToTarget(NetLockedTarget, DeltaTime);//锁定目标的状态下，角色平滑转向到敌人
		}
	}
	if (!HasAuthority())
	{
		Server_UpdateRotation(DeltaTime);
	}
}




void ANeryCharacter::SmoothRotateToTarget(AActor* TargetActor, float DeltaTime)
{
	//这里通过插值的方式来实现角色平滑旋转到目标身上，这样在锁定敌人的时候，角色的旋转就不会太生硬了。
	if (!TargetActor)return;
	FRotator CurrentRotation = GetActorRotation();
	FVector DirectionToTarget = TargetActor->GetActorLocation() - GetActorLocation();
	DirectionToTarget.Z = 0; //只考虑水平旋转,避免角色在锁定目标时出现抬头或者低头的情况
	FRotator TargetRotation = DirectionToTarget.Rotation();

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotateSpeed);
	//只对yaw进行旋转
	NewRotation.Pitch = 0.f;
	NewRotation.Roll = 0.f;
	SetActorRotation(NewRotation);
}

void ANeryCharacter::SetLockMode(bool bIsLockOn)
{
	GetCharacterMovement()->bOrientRotationToMovement = !bIsLockOn; //角色移动时旋转朝向
	//这里更改，不再硬编码设置角色旋转，这样会导致太生硬，在锁定敌人的瞬间，角色会立马转向敌人，不够自然。
	if (IsLocallyControlled())
	{
		 bIsLockOn_NetWorked = bIsLockOn;
		 OnRep_LockOn();//本地调用，消除延迟
		 NetLockedTarget = GetLockOnTarget();
		 if(!HasAuthority())
		 {
			 Server_SetLockStatus(bIsLockOn);
			 Server_SetLockTarget(GetLockOnTarget());
		 }
	}
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



void ANeryCharacter::Server_SetLockTarget_Implementation(AActor* NewTarget)
{
	NetLockedTarget = NewTarget;
}


void ANeryCharacter::Server_SetLockStatus_Implementation(bool bIsLockOn)
{
	bIsLockOn_NetWorked = bIsLockOn;//锁定状态更新后就调用这个onrep_lockon
}

void ANeryCharacter::Multicast_PlayAttackMontage_Implementation(const int32 Index)
{
	if (!IsLocallyControlled())
	{
		if (CharacterDataAsset->AttackMontages.IsValidIndex(Index))
		{
			PlayAnimMontage(CharacterDataAsset->AttackMontages[Index]);
			
		}
	}
}

void ANeryCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	//将当前的锁定状态进行网络复制
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANeryCharacter, bIsLockOn_NetWorked);
	DOREPLIFETIME(ANeryCharacter, AttackState);
	DOREPLIFETIME(ANeryCharacter, NetLockedTarget);
}

void ANeryCharacter::OnRep_LockOn()
{
	LinkAnimTiming(bIsLockOn_NetWorked);
	// 同时更新移动组件旋转设置，确保视觉表现一致
	GetCharacterMovement()->bOrientRotationToMovement = !bIsLockOn_NetWorked;
}

ECharacterAttackState ANeryCharacter::GetAttackState_Implementation()
{
	return AttackState;
}

void ANeryCharacter::Server_UpdateRotation_Implementation(float DeltaTime)
{
	SmoothRotateToTarget(NetLockedTarget, DeltaTime);
}


void ANeryCharacter::Server_ReceiveAttackInput_Implementation()
{
	if (AttackState == ECharacterAttackState::None && ClickTime < CharacterDataAsset->AttackMontages.Num())
	{//在没有攻击状态的时候才可以播放攻击动画
		AttackState = ECharacterAttackState::Attacking;
		Multicast_PlayAttackMontage(ClickTime);
		ClickTime++;
	}
	else
	{
		//在attacking的状态下按下攻击键，表示当前玩家需要继续连招.
		if(bInputBuffered == false && ClickTime < CharacterDataAsset->AttackMontages.Num())
		{
			bInputBuffered = true;
		}

		if (AttackState == ECharacterAttackState::Attacking && ClickTime >= CharacterDataAsset->AttackMontages.Num())
		{//保底逻辑
			AttackState = ECharacterAttackState::None;
			ClickTime = 0;
			bInputBuffered = false;
		}
	}
}

bool ANeryCharacter::GetIsLockOn()
{
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(GetController()))
	{
		return PC->IsLocked();
	}
	return false;
}

AActor* ANeryCharacter::GetLockOnTarget()
{
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(GetController()))
	{
		return PC->GetLockedActor();
	}
	return nullptr;
}

void ANeryCharacter::SaveNotify()//动画通知的函数
{//在这里面应该需要先判断是否进行了攻击输入
	if (AttackState == ECharacterAttackState::Attacking)
	{
		//这里虽然只要动画蒙太奇播放，这个通知就会被触发，但是我怎么知道当前按了攻击输入
		//换个思路，我不如直接在这里将状态设置为None，这样从这个触发时刻开始后，又可以开始接口攻击播放动画。
		//而且在这个通知节点后面，只有有一下点击了输入，在下面的处理输入回掉函数中通过判断当前的clicktime来
		//播放下一个动画，这样既不会重复播放第一个动画，也能实现连续攻击的逻辑
		AttackState = ECharacterAttackState::None;
		//判断玩家是否提前就按下了攻击输入，并且缓存到了bInputBuffered中，如果是的话，就直接调用ReceiveAttackInput来播放下一个攻击动画
		if (bInputBuffered == true )
		{
			bInputBuffered = false;
			ReceiveAttackInput();
		}
		Weapon->IgnoreActors.Empty();
		return;
	}
}

void ANeryCharacter::ResetNotify()
{//这个通知放到动画的最后，只要触发了这个通知，就说明当前攻击动画播放完了
	//这里不考虑在该通知下还按下攻击输入的情况，这个就单纯的用来重置攻击次数和攻击状态。
	ClickTime = 0;
	AttackState = ECharacterAttackState::None;
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
	Weapon->IgnoreActors.Empty();
	
}

void ANeryCharacter::ReceiveAttackInput()//接收到攻击输入的回调函数
{
	if (CharacterDataAsset->AttackMontages.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("没有设置攻击动画蒙太奇"));
		return;
	}

	if (IsLocallyControlled())
	{//本地
		HandleAttackLogic();
	}
	if (!HasAuthority())
	{//服务器
		Server_ReceiveAttackInput();//在本地调用完攻击逻辑后，就通知服务器来处理攻击逻辑，这样服务器就能知道当前玩家的攻击状态和攻击次数，从而来决定是否可以播放下一个攻击动画，实现连招的逻辑。

	}
	
}

void ANeryCharacter::HandleAttackLogic()
{
	//本地播放逻辑
	if (AttackState == ECharacterAttackState::None && ClickTime < CharacterDataAsset->AttackMontages.Num())
	{//在没有攻击状态的时候才可以播放攻击动画
		AttackState = ECharacterAttackState::Attacking;
		//在根动画蒙太奇动画播放时，禁止物理旋转
		GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
		PlayAnimMontage(CharacterDataAsset->AttackMontages[ClickTime]);
		ClickTime++;
		
	}
	else
	{
		//在attacking的状态下按下攻击键，表示当前玩家需要继续连招.
		if(bInputBuffered == false && ClickTime < CharacterDataAsset->AttackMontages.Num())
		{
			bInputBuffered = true;
		}

		if (AttackState == ECharacterAttackState::Attacking && ClickTime >= CharacterDataAsset->AttackMontages.Num())
		{//保底逻辑
			AttackState = ECharacterAttackState::None;
			ClickTime = 0;
			bInputBuffered = false;
		}
	}
}

void ANeryCharacter::Server_SetMaxWalkSpeed_Implementation(float NewMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
}
