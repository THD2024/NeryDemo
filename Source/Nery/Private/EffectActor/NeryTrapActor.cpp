// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/NeryTrapActor.h"
#include"AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include"GameplayEffect.h"

void ANeryTrapActor::BeginPlay()
{
	Super::BeginPlay();
}

void ANeryTrapActor::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("碰撞开始"));

	//应用游戏效果到重叠的角色上
	if (GameplayEffectClass == nullptr)
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
		EffectContext.AddSourceObject(GameplayEffectClass);//实际效果来源对象
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			SpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite ? IsInfinite = true : IsInfinite = false;
		}
		FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		if (IsInfinite)
		{
			ActiveGameplayEffects.Add(ASC, ActiveHandle);
		}

	}

}

void ANeryTrapActor::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GameplayEffectClass == nullptr)
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
