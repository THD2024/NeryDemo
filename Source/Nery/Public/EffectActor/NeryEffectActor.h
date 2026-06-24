// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"GameplayEffect.h"
#include "NeryEffectActor.generated.h"



UCLASS()
class NERY_API ANeryEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeryEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//这个保留在父类
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;
	//这个保留在父类
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> SphereComponent;

	//这个保留在父类
	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;//根据当前actor的实际效果添加对应的游戏效果，但是始终保证数组中只有一个游戏效果

	UFUNCTION()//这个保留在父类
	virtual void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()//这个保留在父类
	virtual void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
