// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include"NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include"GameplayEffect.h"
#include "Components/BoxComponent.h"
#include "NeryProjectileActor.generated.h"

UCLASS()
class NERY_API ANeryProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeryProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector BoxSize = FVector(0,0,0);//默认为000
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Box;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float LifeSpan = 5.f;//生命周期默认为5.f
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> Effect;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> PAGameplayEffect;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector PreviousLocation = GetActorLocation();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector CurrentLocation = GetActorLocation();
	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UAudioComponent> ProjectileSoundComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Destroyed() override;
	
	UFUNCTION(BlueprintCallable)
	void BoxTrace(const ECollisionChannel& CollisionChannel);
};
