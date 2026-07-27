// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/NeryRadiusDamageActor.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include"GameFramework/Character.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

// Sets default values
ANeryRadiusDamageActor::ANeryRadiusDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("CollisionSphere"));
	SetRootComponent(CollisionSphere);
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(FName("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(CollisionSphere);

}

// Called when the game starts or when spawned
void ANeryRadiusDamageActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3.f);
}

// Called every frame
void ANeryRadiusDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANeryRadiusDamageActor::FireCharacter(ACharacter* OverlapCH,float Strength)
{
	FVector Dir = OverlapCH->GetActorLocation() - GetActorLocation();
	Dir.Normalize();
	FVector FinalDir = Dir* Strength;
	OverlapCH->LaunchCharacter(FinalDir,true,true);
}

void ANeryRadiusDamageActor::SphereTrace(const ECollisionChannel& CollisionChannel)
{
	//该函数触发条件在蓝图中写，这里是这种半径伤害的父类，只写最基本的逻辑
	//这里面分为内环和外环，更具不同的环来决定不同的gameplayeffect。
	TArray<AActor*> OverlappedActors;
	OverlappedActors.Add(GetOwner());//在能力激活的时候设置owner
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(OuterRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(OverlappedActors);
	TArray<FOverlapResult> OverlapResults;
	
	if (!GetWorld())return;
	bool bHit = GetWorld()->OverlapMultiByChannel(OverlapResults,GetActorLocation(),FQuat::Identity,CollisionChannel,CollisionShape,QueryParams);
	if (bHit)
	{
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			float DistancetoActor = GetDistanceTo(OverlapResult.GetActor());
			FHitResult HitResult;
			//将受击点设置在一半的位置上
			HitResult.ImpactPoint = OverlapResult.GetActor()->GetActorLocation() + FVector(0,0,50.f);
			if (DistancetoActor <= InnerRadius)
			{
				if (InnerGameplayEffect)
				{
					UNeryBlueprintFunctionLibrary::ApplyEffectToActor(OverlapResult.GetActor(),InnerGameplayEffect,HitResult);
				}
			}
			else
			{//这里不需要判断是不是在外圈外面，因为这里的检测结果并没有保留，每次都是新的检测结果，所以能检测到就证明至少在外圈内
				if (OuterGameplayEffect)
				{
					UNeryBlueprintFunctionLibrary::ApplyEffectToActor(OverlapResult.GetActor(),OuterGameplayEffect,HitResult);
				}
				//这里还要写击退效果,通过launcher character来实现击退
				if (ACharacter* OverlapCH = Cast<ACharacter>(OverlapResult.GetActor()))
				{
					FireCharacter(OverlapCH,FireStrength);
				};
			}
		}
	}
}


