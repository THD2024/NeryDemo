// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "NeryAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API UNeryAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
protected:
	static UNeryAssetManager& Get();

	virtual void StartInitialLoading() override;
};
