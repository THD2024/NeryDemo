// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NeryBaseCharacter.h"
#include "NeryCharacter.generated.h"

class ANeryPlayerState;

/**
 * 
 */
UCLASS()
class NERY_API ANeryCharacter : public ANeryBaseCharacter
{
	GENERATED_BODY()

public:
	ANeryCharacter();

	void InitHUD();

	void SetMaxWalkSpeed(float NewMaxWalkSpeed);

	UFUNCTION(BlueprintImplementableEvent)
	void LinkAnimTiming(bool IsLockOn);

	UFUNCTION(BlueprintCallable)
	bool GetIsLockOn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RunMaxWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RunNormalWalkSpeed = 300.f;

protected:
	virtual void BeginPlay() override;
	
	void PossessedBy(AController* NewController) override;

	void OnRep_PlayerState() override;
	
	void InitASCandAttribute();

	UFUNCTION(Server, Reliable)
	void Server_SetMaxWalkSpeed(float NewMaxWalkSpeed);
	
};
