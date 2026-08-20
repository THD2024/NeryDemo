// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"UI/Controller/OverlayWidgetController.h"
#include"UI/Widget/NeryUserWidget.h"
#include"Components/WidgetComponent.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include "EffectActor/Weapon.h"
#include "Kismet/KismetMathLibrary.h"


AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UNeryAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UNeryAttributeSet>(TEXT("AttributeSet"));
	WarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("WarpingComponent"));
	bReplicates = true;
	SetReplicateMovement(true);
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	NetUpdateFrequency = 100.f;//通用
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());//显式添加属性集子对象到能力系统组件中
	//创建锁定目标反馈组件
	LockTargetFeedbackWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockTargetFeedbackWidget"));
	LockTargetFeedbackWidget->SetupAttachment(GetMesh(),FName("LockTargetFeedbackSocket"));
	LockTargetFeedbackWidget->SetVisibility(false);
	Tags.Add(TEXT("Enemy"));
	TeamId = FGenericTeamId(0);//0表示敌人阵营
}

void AEnemyCharacter::BindCallbacks()
{
	if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
	{
		PoiseStatusDelegate.BindUObject(this,&AEnemyCharacter::UpdatePoiseStatus);
		if(UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &AEnemyCharacter::OnHealthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddUObject(this, &AEnemyCharacter::OnMaxHealthDelegate);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetPoiseAttribute()).AddUObject(this,&AEnemyCharacter::OnPoiseChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxPoiseAttribute()).AddUObject(this,&AEnemyCharacter::OnMaxPoiseChanged);
			OnEnemyHealthChanged.Broadcast(AS->GetHealth());//广播初始值
			MaxHealthDelegate.Broadcast(AS->GetMaxHealth());
			MaxPoiseDelegate.Broadcast(AS->GetMaxPoise());
			PoiseDelegate.Broadcast(AS->GetPoise());
		}
	}
}

bool AEnemyCharacter::ActivateEnemyAbilityByTag(const FGameplayTag& Tag)
{//用在行为树task中用来激活敌人技能
	if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (Tag.IsValid())
		{
			return ASC->ActiveEnemyAbilityByDynamicTag(Tag);
		}
	}
	return false;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		if (HasAuthority())
		{
			InitAttribute();
			SpawnWeapon();
		}
		
		InitWidget();
		BindCallbacks();
		
	}
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitAttribute();
		GiveEnemyAbilities();//赋予能力
	}
	
}

void AEnemyCharacter::GiveEnemyAbilities()
{
	if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
	{
		for (auto Ability : EnemyAbilities)
		{
			if (Ability.IsValid())
			{
				ASC->GiveCharacterOwningAbility(Ability);
			}
		}
	}
}

void AEnemyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	OnEnemyHealthChanged.Broadcast(Data.NewValue);
}

void AEnemyCharacter::OnMaxHealthDelegate(const FOnAttributeChangeData& Data)
{
	MaxHealthDelegate.Broadcast(Data.NewValue);
}

void AEnemyCharacter::LockTargetFeedBack_Implementation()
{
	LockTargetFeedbackWidget->SetVisibility(true);
}

void AEnemyCharacter::UnLockTargetFeedBack_Implementation()
{
	LockTargetFeedbackWidget->SetVisibility(false);
}

FTransform AEnemyCharacter::GetWeaponLocation_Implementation()
{
	FTransform Transform = FTransform();
	if (Weapon && Weapon->ScenePoint)
	{
		Transform.SetLocation(Weapon->GetActorLocation());
		FVector ForWard = GetActorForwardVector();
		FVector SocketRightVector = Weapon->ScenePoint->GetRightVector();
		FRotator Rotation = UKismetMathLibrary::MakeRotFromXY(ForWard,SocketRightVector);
		Transform.SetRotation(Rotation.Quaternion());
	}
	return Transform;
}


void AEnemyCharacter::UpdateWarpTarget_Implementation(FName TargetName, FVector TargetLocation, FRotator TargetRotation)
{
	// 核心 ：向组件注册或更新一个“标记点”
	WarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TargetName, TargetLocation, TargetRotation);
}

bool AEnemyCharacter::GetEnemyPoiseStatus_Implementation()
{
	return IsRecovering;
}

void AEnemyCharacter::OnMaxPoiseChanged(const FOnAttributeChangeData& Data)
{
	MaxPoiseDelegate.Broadcast(Data.NewValue);
}

void AEnemyCharacter::OnPoiseChanged(const FOnAttributeChangeData& Data)
{
	PoiseDelegate.Broadcast(Data.NewValue);
	if (Data.NewValue <= 0)
	{
		bPoiseStatus = false;
		PoiseStatusDelegate.Execute(bPoiseStatus);
	}
	else 
	{//只要霸体不为0，就是霸体状态
		bPoiseStatus = true;
		if (FMath::IsNearlyEqual(Data.NewValue, 100.f))
		{
			IsRecovering = false;
		}
	}
	
	
}

void AEnemyCharacter::UpdatePoiseStatus(bool InbPoiseStatus)
{
	if (!bPoiseStatus && !IsRecovering)
	{
		//这里执行恢复效果，同时添加破防标签,在属性中进行拦截，防止攻击会减少霸体恢复
		IsRecovering = true;
		
		UNeryBlueprintFunctionLibrary::ApplyBasicEffectToSelf(this,PoiseRecoverEffect);
	}
}





