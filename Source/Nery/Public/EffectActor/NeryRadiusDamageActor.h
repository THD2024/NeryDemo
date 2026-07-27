// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "NeryRadiusDamageActor.generated.h"

//如果以后从这个类扩展的话，只需要将spheretrace设置为virtual，比如手雷这种.
UCLASS()
class NERY_API ANeryRadiusDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeryRadiusDamageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float InnerRadius = 0.f;//默认为0.f，蓝图中进行更改,表示内圈的半径。
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float OuterRadius = 0.f;//默认为0.f，蓝图中进行更改,表示内圈的半径。
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float FireStrength = 0.f;//击退速度，越大，击飞越远
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InnerGameplayEffect;//内环
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> OuterGameplayEffect;//外环
	
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void FireCharacter(ACharacter* OverlapCH,float Strength);

	UFUNCTION(BlueprintCallable)
	void SphereTrace(const ECollisionChannel& CollisionChannel );
	
};
