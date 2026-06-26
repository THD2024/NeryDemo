// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/NeryBuffEffectActor.h"
#include "Blueprint/UserWidget.h"
#include"Data/ItemBagDataAsset.h"
#include"Kismet/GameplayStatics.h"
#include"PlayerController/NeryPlayerController.h"
#include"Interface/CombatInterface.h"

void ANeryBuffEffectActor::BeginPlay()
{
	Super::BeginPlay();
	bIsOverlaped = true;
}

void ANeryBuffEffectActor::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BuffActor"));
	if (OtherActor->ActorHasTag(FName("Enemy")))return;//保证进来的是角色
	OverlapActor = OtherActor;
	bIsOverlaped = true;
	if (!PC)
	{
		PC = Cast<ANeryPlayerController>(OtherActor->GetInstigatorController());
		if (PC && PC->OnPickAction.IsBoundToObject(this) == false)
		{
			PC->OnPickAction.AddUObject(this, &ANeryBuffEffectActor::AddActorToItemBag);
		}
	}
	
	//生成提示小组件
	if (!PickWidget)
	{
		PickWidget = CreateWidget<UUserWidget>(GetWorld(), PickWidgetClass);
		PickWidget->AddToViewport();
		ItemBag->GetSpecificItemInfoByTag(BuffTag);
		OnSetWidgetTiming(ItemBag->GetSpecificItemInfoByTag(BuffTag));
	}
	else
	{
		PickWidget->SetVisibility(ESlateVisibility::Visible);
	}

}

void ANeryBuffEffectActor::SphereEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Enemy")))return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BuffActor endoverlap"));
	OverlapActor = nullptr;
	bIsOverlaped = false;
	if (PickWidget)
	{
		PickWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ANeryBuffEffectActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PC)
	{
		PC->OnPickAction.RemoveAll(this);
	}
}



void ANeryBuffEffectActor::AddActorToItemBag()
{
	//由按键输入来调用,这里表示已经按下拾取键，暂时不考虑拾取上限的问题。这里被触发
	if (!bIsOverlaped)return;
	//只有在检测范围内才能进行下面的拾取逻辑
		if (OverlapActor && OverlapActor->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_CallAddBuffNumber(OverlapActor, BuffTag);
		}
		
		//写添加逻辑
		PickWidget->RemoveFromParent();//离开就直接销毁提示组件
		Destroy();//拾取完后需要将对象销毁
	
}
