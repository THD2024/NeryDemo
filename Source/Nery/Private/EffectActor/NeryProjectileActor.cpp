// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/NeryProjectileActor.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"

// Sets default values
ANeryProjectileActor::ANeryProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	SetRootComponent(Sphere);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
	Effect = CreateDefaultSubobject<UNiagaraComponent>(FName("Effect"));
	Effect->SetupAttachment(RootComponent);
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 1000.f;//投掷物速度
	ProjectileMovementComponent->MaxSpeed = 1000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;//直线飞行，没有速度
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void ANeryProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ANeryProjectileActor::OnBoxOverlap);
	SetLifeSpan(LifeSpan);
}

// Called every frame
void ANeryProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANeryProjectileActor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//这里用来写伤害逻辑
	if (OtherActor->ActorHasTag(FName("Enmey")))
	{
		//这里会自动触发gameplaycue实现集中效果
		UNeryBlueprintFunctionLibrary::ApplyEffectToActor(OtherActor,PAGameplayEffect,SweepResult);
	}
	
}

