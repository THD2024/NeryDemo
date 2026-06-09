// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NeryGameModeBase.generated.h"

/**
 * 
 */
class UCharacterDataAsset;

UCLASS()
class NERY_API ANeryGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	TObjectPtr<UCharacterDataAsset> CharacterInfo;
};
