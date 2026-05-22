// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/NeryEffectActor.h"
#include"Components/StaticMeshComponent.h"
#include"Components/SphereComponent.h"
#include"AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ANeryEffectActor::ANeryEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void ANeryEffectActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ANeryEffectActor::SphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ANeryEffectActor::SphereEndOverlap);
}

void ANeryEffectActor::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//应用游戏效果到重叠的角色上
	if (GameplayEffect == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("检查EffecActor数组中是否添加了游戏效果"));
		return;
	}	
	//这里数组中只有一个游戏效果
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (ASC)
	{
		bool IsInfinite = false;
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddInstigator(this, this);//实际发起者和物理接触者
		EffectContext.AddSourceObject(GameplayEffect);//实际效果来源对象
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffect->GetClass(), 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			GameplayEffect->DurationPolicy == EGameplayEffectDurationType::Infinite ? IsInfinite = true : IsInfinite = false;
		}
		FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (IsInfinite)
		{
			ActiveGameplayEffects.Add( ASC, ActiveHandle);
		}
		
	}

}

void ANeryEffectActor::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GameplayEffect == nullptr) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("检查EffecActor数组中是否添加了游戏效果"));
		return;
	}
		//移除无限游戏效果
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		//这里不要直接解引用，否则如果没有找到对应的ASC会导致崩溃
		FActiveGameplayEffectHandle* InfiniteActiveHandle = ActiveGameplayEffects.Find(ASC);
		if (InfiniteActiveHandle && InfiniteActiveHandle->IsValid())
		{
			ASC->RemoveActiveGameplayEffect(*InfiniteActiveHandle);
			ActiveGameplayEffects.Remove(ASC);
		}
}



