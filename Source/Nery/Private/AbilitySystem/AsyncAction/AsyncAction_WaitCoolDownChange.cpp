// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncAction/AsyncAction_WaitCoolDownChange.h"

UAsyncAction_WaitCoolDownChange* UAsyncAction_WaitCoolDownChange::WaitCoolDownChange(UAbilitySystemComponent* InASC,
	const FGameplayTag& InCoolDownTag)
{
	UAsyncAction_WaitCoolDownChange* Action = NewObject<UAsyncAction_WaitCoolDownChange>();
	Action->ASC = InASC;
	Action->CoolDownTag = InCoolDownTag;
	return Action;
}

void UAsyncAction_WaitCoolDownChange::Activate()
{
	Super::Activate();
	if (!ASC || !CoolDownTag.IsValid())return;
	//绑定到对应的冷却标签上面
	ASC->RegisterGameplayTagEvent(CoolDownTag,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&UAsyncAction_WaitCoolDownChange::OnTagChanged);
}

void UAsyncAction_WaitCoolDownChange::SetReadyToDestroy()
{
	if (!ASC)return;
	ASC->RegisterGameplayTagEvent(CoolDownTag,EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	Super::SetReadyToDestroy();
}

void UAsyncAction_WaitCoolDownChange::EndTask()
{
	SetReadyToDestroy();
}

void UAsyncAction_WaitCoolDownChange::OnTagChanged(const FGameplayTag InTag, int32 NewCount)
{
	//因为这个函数我只打算在ui中调用，且是在确认是当前Ui装的技能后在调用这个，所以这里只会监听特定的标签，不会出现同时监听多个或者是标签被添加到多个对象身上
	//当标签变化后,判断当前是添加还是移除
	if (!ASC)return;
	if (NewCount > 0)
	{
		float DurationTime = 0.f;
		float RemainingTime = 0.f;
		FGameplayEffectQuery Query;
		FGameplayTagContainer TagContainer; 
		TagContainer.AddTag(InTag);
		Query.OwningTagQuery.MakeQuery_MatchAllTags(TagContainer);
		TArray<TPair<float,float>> EffectTime = ASC->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (EffectTime.IsEmpty())return;
		DurationTime = EffectTime.Last().Value;
		RemainingTime = EffectTime.Last().Key;
		CoolDownDelegate.Broadcast(DurationTime,RemainingTime);
	}
	else
	{
		CoolDownDelegate.Broadcast(0,0);//当标签被移除后，直接冷却归0
	}
	
}
