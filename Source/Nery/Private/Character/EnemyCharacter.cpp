// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include"AbilitySystem/NeryAbilitySystemComponent.h"
#include"UI/Controller/OverlayWidgetController.h"
#include"UI/Widget/NeryUserWidget.h"
#include"Components/WidgetComponent.h"
#include"AbilitySystem/NeryAttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UNeryAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UNeryAttributeSet>(TEXT("AttributeSet"));
	bReplicates = true;
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	NetUpdateFrequency = 100.f;//通用
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());//显式添加属性集子对象到能力系统组件中
	//创建锁定目标反馈组件
	LockTargetFeedbackWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockTargetFeedbackWidget"));
	LockTargetFeedbackWidget->SetupAttachment(GetMesh(),FName("LockTargetFeedbackSocket"));
	LockTargetFeedbackWidget->SetVisibility(false);
}

void AEnemyCharacter::BindCallbacks()
{
	if (UNeryAbilitySystemComponent* ASC = Cast<UNeryAbilitySystemComponent>(AbilitySystemComponent))
	{
		if(UNeryAttributeSet* AS = Cast<UNeryAttributeSet>(AttributeSet))
		{
			ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddUObject(this, &AEnemyCharacter::OnHealthChanged);
			OnEnemyHealthChanged.Broadcast(AS->GetHealth());//广播初始值
		}
	}
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
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
	}
}

void AEnemyCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	OnEnemyHealthChanged.Broadcast(Data.NewValue);
}

void AEnemyCharacter::LockTargetFeedBack_Implementation()
{
	LockTargetFeedbackWidget->SetVisibility(true);
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, TEXT("LOCK TARGET"));
}

void AEnemyCharacter::UnLockTargetFeedBack_Implementation()
{
	LockTargetFeedbackWidget->SetVisibility(false);
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, TEXT("UNLOCK TARGET"));

}



