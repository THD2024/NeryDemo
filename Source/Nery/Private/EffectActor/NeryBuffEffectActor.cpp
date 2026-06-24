// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/NeryBuffEffectActor.h"
#include "Blueprint/UserWidget.h"
#include"Data/ItemBagDataAsset.h"
#include"Kismet/GameplayStatics.h"
#include"PlayerController/NeryPlayerController.h"

void ANeryBuffEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void ANeryBuffEffectActor::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BuffActor"));
	if (OtherActor->ActorHasTag(FName("Enemy")))return;//保证进来的是角色
	if (!PC)
	{
		PC = Cast<ANeryPlayerController>(OtherActor->GetInstigatorController());
		if (PC)
		{
			PC->OnPickAction.BindUObject(this, &ANeryBuffEffectActor::AddActorToItemBag);
		}
	}
	
	IsOverlaped = true;
	//生成提示小组件
	if (!PickWidget)
	{
		PickWidget = CreateWidget<UUserWidget>(GetWorld(), PickWidgetClass);
		//生成位置在event construct函数后面设置在蓝图中设置。
	}

}

void ANeryBuffEffectActor::SphereEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag(FName("Enemy")))return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BuffActor endoverlap"));
	IsOverlaped = false;
	PickWidget->RemoveFromParent();//离开就直接销毁提示组件
}

void ANeryBuffEffectActor::AddActorToItemBag()
{
	//由按键输入来调用,这里表示已经按下拾取键，暂时不考虑拾取上限的问题。这里被触发
	if (IsOverlaped == true)
	{//只有在检测范围内才能进行下面的拾取逻辑
		//写添加逻辑
		ItemBag->AddItem(BuffTag);
		IsOverlaped = false;
		PickWidget->RemoveFromParent();//离开就直接销毁提示组件
		Destroy();//拾取完后需要将对象销毁
	}
}
