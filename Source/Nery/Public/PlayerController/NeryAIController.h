// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NeryAIController.generated.h"

/**
 * 
 */
UCLASS()
class NERY_API ANeryAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANeryAIController();
	
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()//绑定感知事件的回调函数
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	TObjectPtr<UAISenseConfig_Sight> SenseConfig;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	float AISightRadius = 2000.f;//视角半径
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	float AILoseSightRadius = 2500.f;//追击半径
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	float VisionDegree = 60.f;//视角半角
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	float AIMaxAge = 5.f;//对于看到的对象在消失后的记忆事件，可以通过内置的expired变量来判断是否遗忘
};
