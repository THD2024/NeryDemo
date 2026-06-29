// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include"Data/CharacterDataAsset.h"
#include"Data/WidgetSlotTagInfo.h"
#include "NeryGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API ANeryGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	TObjectPtr<UCharacterDataAsset> CharacterInfo;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	TObjectPtr<UCharacterDataAsset> EnemyBasicInfo;

	UPROPERTY(EditAnywhere, Category = "CharacterInfo")
	TSubclassOf<UWidgetSlotTagInfo> WidgetTagInfoClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UWidgetSlotTagInfo>WidgetTagInfo;

};
