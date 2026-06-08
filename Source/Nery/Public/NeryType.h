// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NeryType.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class ECharacterAttackState : uint8
{
	Attacking,
	None
};

class NERY_API NeryType
{
public:
	NeryType();
	~NeryType();
};
