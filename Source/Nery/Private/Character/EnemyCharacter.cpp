// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

#include "NiagaraGPUInstanceCountManager.h"
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
		if(UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &AEnemyCharacter::OnHealthChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddUObject(this, &AEnemyCharacter::OnMaxHealthDelegate);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetPoiseAttribute()).AddUObject(this,&AEnemyCharacter::OnPoiseChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxPoiseAttribute()).AddUObject(this,&AEnemyCharacter::OnMaxPoiseChanged);
			OnEnemyHealthChanged.Broadcast(AS->GetHealth());//广播初始值
			MaxHealthDelegate.Broadcast(AS->GetMaxHealth());
		}
	}
}

void AEnemyCharacter::ActivateEnemyAbilityByTag(const FGameplayTag& Tag)
{//用在行为树task中用来激活敌人技能
	if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (Tag.IsValid())
		{
			ASC->ActiveAbilityByDynamicTag(Tag);
		}
	}
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
		FRotator Rotation = Weapon->GetActorRotation();
		Rotation.Pitch = 0.0f;
		Transform.SetRotation(Rotation.Quaternion());
	}
	return Transform;
}

void AEnemyCharacter::OnMaxPoiseChanged(const FOnAttributeChangeData& Data)
{
	MaxPoiseDelegate.Broadcast(Data.NewValue);
}

void AEnemyCharacter::OnPoiseChanged(const FOnAttributeChangeData& Data)
{
	PoiseDelegate.Broadcast(Data.NewValue);
}



