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
	ANeryBuffEffectActor();

	virtual void BeginPlay() override;

	virtual void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	virtual void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddActorToItemBag();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetWidgetTiming(const FItemInformation& ItemInfo);
	
	UFUNCTION(Server,Reliable)
	void Server_AddActorToItemBag();

protected:

	UPROPERTY()
	bool bIsOverlaped = false;

	UPROPERTY()
	ANeryPlayerController* PC;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag BuffTag;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemBagDataAsset> ItemBag;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PickWidgetClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UUserWidget> PickWidget;

	UPROPERTY()
	AActor* OverlapActor;
};
