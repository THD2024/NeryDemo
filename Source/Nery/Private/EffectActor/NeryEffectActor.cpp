// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/NeryEffectActor.h"
#include"Components/StaticMeshComponent.h"
#include"Components/SphereComponent.h"


ANeryEffectActor::ANeryEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
}

void ANeryEffectActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ANeryEffectActor::SphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ANeryEffectActor::SphereEndOverlap);
}

void ANeryEffectActor::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{}

void ANeryEffectActor::SphereEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{}




