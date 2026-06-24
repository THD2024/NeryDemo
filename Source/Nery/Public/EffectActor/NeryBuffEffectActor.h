// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectActor/NeryEffectActor.h"
#include"Data/ItemBagDataAsset.h"
#include "NeryBuffEffectActor.generated.h"

struct FGameplayTag;
class ANeryPlayerController;
/**
 * 
 */
UCLASS()
class NERY_API ANeryBuffEffectActor : public ANeryEffectActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	virtual void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	void AddActorToItemBag();

protected:
	bool IsOverlaped = false;

	UPROPERTY()
	ANeryPlayerController* PC;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag BuffTag;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemBagDataAsset> ItemBag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PickWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUserWidget> PickWidget;
};
