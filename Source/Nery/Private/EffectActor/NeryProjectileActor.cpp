// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/NeryProjectileActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"

// Sets default values
ANeryProjectileActor::ANeryProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	
	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	RootComponent = Box;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
	Effect = CreateDefaultSubobject<UNiagaraComponent>(FName("Effect"));
	Effect->SetupAttachment(RootComponent);
	ProjectileSoundComponent = CreateDefaultSubobject<UAudioComponent>(FName("ProjectileSoundComponent"));
	ProjectileSoundComponent->SetupAttachment(RootComponent);
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 1000.f;//投掷物速度
	ProjectileMovementComponent->MaxSpeed = 1000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;//直线飞行，没有重量
	ProjectileMovementComponent->bRotationFollowsVelocity = false;//不跟随速度旋转
	// ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	
}

// Called when the game starts or when spawned
void ANeryProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	if (GetInstigator())
	{
		ProjectileMovementComponent->Velocity = GetInstigator()->GetActorForwardVector()*ProjectileMovementComponent->InitialSpeed;
	}
}

// Called every frame
void ANeryProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANeryProjectileActor::Destroyed()
{
	ActorsToIgnore.Empty();
	Super::Destroyed();
}


void ANeryProjectileActor::BoxTrace(const ECollisionChannel& CollisionChannel)
{
	if (!HasAuthority())return;//伤害只允许服务器来判定
	ActorsToIgnore.Add(GetOwner());//在能力激活的时候设置owner
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(BoxSize);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	TArray<FHitResult> HitResults;
	CurrentLocation = GetActorLocation();
	
	if (GetWorld())
	{
		bool bHit = GetWorld()->SweepMultiByChannel(HitResults,PreviousLocation,CurrentLocation,
			FQuat::Identity,CollisionChannel,CollisionShape,QueryParams);
		if (bHit)
		{
			for (auto& HitResult : HitResults)
			{
				UNeryBlueprintFunctionLibrary::ApplyEffectToTarget(GetOwner(),HitResult.GetActor(),PAGameplayEffect,HitResult,true);
				ActorsToIgnore.Add(HitResult.GetActor());
			}
		}
		PreviousLocation = CurrentLocation;
	}
	
}

