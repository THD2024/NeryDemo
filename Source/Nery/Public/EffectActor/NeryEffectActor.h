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

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UNiagaraComponent> NiagaraComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<class UGameplayEffect> GameplayEffectClass;//根据当前actor的实际效果添加对应的游戏效果，但是始终保证数组中只有一个游戏效果

	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:	
	UPROPERTY()//这里需要添加UPROPERTY()宏，否则如果当某个角色在范围内死亡，这个角色的AbilitySystemComponent就会被销毁
		//但是反射系统并不知道这个角色的AbilitySystemComponent被销毁了，所以在结束重叠事件中就会尝试访问一个已经被销毁的对象，导致崩溃
	TMap< UAbilitySystemComponent*, FActiveGameplayEffectHandle> ActiveGameplayEffects;

};
