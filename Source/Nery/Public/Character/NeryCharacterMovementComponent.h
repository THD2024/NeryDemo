// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NeryCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API UNeryCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	
protected:
	virtual void ProcessClientTimeStampForTimeDiscrepancy(float ClientTimeStamp, FNetworkPredictionData_Server_Character& ServerData) override;
};
