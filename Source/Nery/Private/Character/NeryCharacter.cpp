// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryCharacter.h"
#include"PlayerState/NeryPlayerState.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"PlayerController/NeryPlayerController.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"UI/Controller/WidgetController.h"
#include"Data/CharacterDataAsset.h"
#include"EffectActor/Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include"UI/HUD/NeryHUD.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include"Data/ItemBagDataAsset.h"
#include "Perception/AISense_Sight.h"


ANeryCharacter::ANeryCharacter(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	//防止角色跟随控制器旋转	
	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSourceComponent"));
	PerceptionStimuliSourceComponent->bAutoRegister = true;
	PerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	TeamId = FGenericTeamId(1);//1表示角色阵营
	
	bReplicates = true;
	SetReplicateMovement(true);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = RunNormalWalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true; //角色移动时旋转朝向
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
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

void ANeryCharacter::Server_AddBuffNumberByTag_Implementation(const FGameplayTag& InTag, AActor* Interactor)
{
	for (auto& BuffInfo : BuffNumberBagInfo)
	{
		if (BuffInfo.BuffTag.MatchesTagExact(InTag))
		{
			UE_LOG(LogTemp, Warning, TEXT("Before Add : %d"), BuffInfo.BuffNumber);
			BuffInfo.BuffNumber++;
			break;
		}
	}
	if(IsLocallyControlled())
	{
		TryToBroadBuffNumberInfo();
	}
	if (Interactor)
	{
		Interactor->Destroy();
	}
}

void ANeryCharacter::AddBuffNumberByTag(const FGameplayTag& InTag,AActor*Interactor)
{
	Server_AddBuffNumberByTag(InTag,Interactor);
}

void ANeryCharacter::Server_ReduceBuffNumberByTag_Implementation(const FGameplayTag& InTag)
{
	for (auto& BuffInfo : BuffNumberBagInfo)
	{
		if (BuffInfo.BuffTag.MatchesTagExact(InTag))
		{
			if (BuffInfo.BuffNumber > 0)
			{
				ApplyBuffEffect(InTag);
				BuffInfo.BuffNumber--;
				BuffInfo.BuffNumber = FMath::Max<float>(0.f, BuffInfo.BuffNumber);
			}
			
			if (IsLocallyControlled())
			{
				TryToBroadBuffNumberInfo();
			}
			return;
		}
	}
}

void ANeryCharacter::ReduceBuffNumberByTag(const FGameplayTag & InTag)
{
	Server_ReduceBuffNumberByTag(InTag);
}



void ANeryCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(GetController()))
	{
		PC->OnLinkAnimTiminig.AddLambda([this](bool IsLockOn) {
			SetLockMode(IsLockOn);//根据当前的锁定状态来设置锁定模式
			});
	}
	if (HasAuthority())
	{
		SpawnWeapon();
	}
	
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
		if (bIsLockOn_NetWorked && NetLockedTarget)
		{
			Server_UpdateRotation(DeltaTime);
		}
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
	InitAttribute();
	if (IsLocallyControlled())//保证了在当前为监听服务器的情况下，也显示Ui
	{
		InitHUD();
		InitAttribute();
	}
	GiveBasicAbilities();
	GiveOwningAbilities();
}

void ANeryCharacter::GiveBasicAbilities()
{
	if (!HasAuthority())return;//这里保证能力只被服务器赋予，防止后面如果别的地方调用。
	if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (CharacterDataAsset)
		{
			if (CharacterDataAsset->BasicAbilitiesClass.Num() > 0)
			{
				ASC->GiveCharacterAbilities(CharacterDataAsset->BasicAbilitiesClass);
			}
		}
	}
}

void ANeryCharacter::GiveOwningAbilities()
{
	if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
	{
		for (const auto& Abilities : OwningAbilities)
		{
			if (Abilities.IsValid())
			{
				ASC->GiveCharacterOwningAbility(Abilities);
			}
		}
	}
	
}


void ANeryCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitASCandAttribute();
	if (IsLocallyControlled())
	{
		InitHUD();//Hud属于表现层，在这里调用初始化Hud的函数，来确保在玩家状态复制到客户端后，客户端的Hud能够正确显示玩家状态的信息。
		//同时这里保证只会初始Hud到当前玩家的屏幕上
		//初始化技能菜单放到了inithud中
	}
	
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

void ANeryCharacter::CallAddBuffNumber_Implementation(const FGameplayTag& InTag, AActor* Interactor)
{
	AddBuffNumberByTag(InTag, Interactor);
}


UItemBagDataAsset* ANeryCharacter::GetItemBag_Implementation()
{
	if (ItemBag)
	{
		return ItemBag;
	}
	return nullptr;
}

TMap<FGameplayTag, int32> ANeryCharacter::GetBuffNumber_Implementation()
{
	TMap<FGameplayTag, int32> Result;
	for (const auto& BagInfo : BuffNumberBagInfo)
	{
		Result.Add(BagInfo.BuffTag, BagInfo.BuffNumber);
	}
	return Result;
}


void ANeryCharacter::Server_SetLockTarget_Implementation(AActor* NewTarget)
{
	NetLockedTarget = NewTarget;
}


void ANeryCharacter::Server_SetLockStatus_Implementation(bool bIsLockOn)
{
	bIsLockOn_NetWorked = bIsLockOn;//锁定状态更新后就调用这个onrep_lockon
}

void ANeryCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	//将当前的锁定状态进行网络复制
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANeryCharacter, bIsLockOn_NetWorked);
	// DOREPLIFETIME(ANeryCharacter, AttackState);
	DOREPLIFETIME(ANeryCharacter, NetLockedTarget);
	DOREPLIFETIME_CONDITION(ANeryCharacter, BuffNumberBagInfo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ANeryCharacter,OwningAbilities, COND_OwnerOnly);
}

void ANeryCharacter::OnRep_LockOn()
{
	LinkAnimTiming(bIsLockOn_NetWorked);
	// 同时更新移动组件旋转设置，确保视觉表现一致
	GetCharacterMovement()->bOrientRotationToMovement = !bIsLockOn_NetWorked;
}

void ANeryCharacter::Server_ApplyBuffEffect_Implementation(const FGameplayTag& InTag)
{
	ApplyBuffEffect(InTag);
}

void ANeryCharacter::OnRep_BuffNumberChanged()
{
	TryToBroadBuffNumberInfo();
}


void ANeryCharacter::Server_UpgradeBasicAttributebyPoints_Implementation(const FGameplayTag& AttributeTag)
{
	UNeryBlueprintFunctionLibrary::AddBasicAttributePoints(this,AttributeTag, this);
}

void ANeryCharacter::ApplyBuffEffect(const FGameplayTag& InTag)
{
	if (ItemBag)
	{
		if (ItemBag->FindSpecificEffectByTag(InTag))
		{
			UNeryBlueprintFunctionLibrary::ApplyBasicEffectToSelf(this, ItemBag->FindSpecificEffectByTag(InTag));
		}
	}
}

void ANeryCharacter::TryToBroadBuffNumberInfo()
{
	if (ANeryPlayerController* PC = Cast<ANeryPlayerController>(GetController()))
	{
		PC->OnBuffNumberAdded.Broadcast();
	}
	
}


void ANeryCharacter::CallUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	if (UNeryAttributeSet* NeryAS = Cast<UNeryAttributeSet>(AttributeSet))
	{
		if (NeryAS->GetAttributePoint() > 0)//判断当前属性点是否是>0，才能进行属性
		{
			Server_UpgradeBasicAttributebyPoints(AttributeTag);
		}
	}
}

TArray<FGameplayTag>  ANeryCharacter::GetCharacterActivateAbilities_Implementation()
{
	if (OwningAbilities.Num() > 0)
	{
		return OwningAbilities;
	}
	return TArray<FGameplayTag>();
}

FTransform ANeryCharacter::GetWeaponLocation_Implementation()
{
	FTransform Transform = FTransform();
	if (Weapon && Weapon->ScenePoint)
	{
		Transform.SetLocation(Weapon->ScenePoint->GetComponentLocation());
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Transform.GetLocation());
		Rotation.Pitch = 0.0f;
		Transform.SetRotation(Rotation.Quaternion());
	}
	if (GetIsLockOn())
	{
		if (AActor* LockActor = GetLockOnTarget())
		{
			FVector TargetLocation = LockActor->GetActorLocation() + FVector(0.f,0.f,50.f);
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),TargetLocation);
			Rotation.Pitch = 0.f;//设置为水平，没有角度
			Transform.SetRotation(Rotation.Quaternion());
		}
	}
	return Transform;
}


void ANeryCharacter::Server_UpdateRotation_Implementation(float DeltaTime)
{
	SmoothRotateToTarget(NetLockedTarget, DeltaTime);
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

void ANeryCharacter::Server_SetMaxWalkSpeed_Implementation(float NewMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
}
