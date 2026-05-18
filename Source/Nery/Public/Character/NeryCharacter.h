// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NeryBaseCharacter.h"
#include "NeryCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API ANeryCharacter : public ANeryBaseCharacter
{
	GENERATED_BODY()

public:
	ANeryCharacter();

	void SetMaxWalkSpeed(float NewMaxWalkSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RunMaxWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RunNormalWalkSpeed = 230.f;

protected:
	virtual void BeginPlay() override;
	
	
};
