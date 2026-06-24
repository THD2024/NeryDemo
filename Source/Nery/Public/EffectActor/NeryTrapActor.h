// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectActor/NeryEffectActor.h"
#include "NeryTrapActor.generated.h"

struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class NERY_API ANeryTrapActor : public ANeryEffectActor
{
	GENERATED_BODY()
public:
	
	
	virtual void BeginPlay() override;

	virtual	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


	virtual void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;


	UPROPERTY()
	TMap<UAbilitySystemComponent*, FActiveGameplayEffectHandle> ActiveGameplayEffects;

};
