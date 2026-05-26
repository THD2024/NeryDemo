// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NeryBaseCharacter.h"
#include "Interface/CombatInterface.h"
#include "EnemyCharacter.generated.h"

struct FOnAttributeChangeData;//记得声明
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyHealthChanged, float, NewHealth);

UCLASS()
class NERY_API AEnemyCharacter : public ANeryBaseCharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	void BindCallbacks();

	UFUNCTION(BlueprintImplementableEvent)
	void InitWidget();

protected:

	void BeginPlay() override;

	void PossessedBy(AController* NewController) override;

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	virtual void LockTargetFeedBack_Implementation() override;
	virtual void UnLockTargetFeedBack_Implementation()override;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyHealthChanged OnEnemyHealthChanged;
	
	UPROPERTY(EditDefaultsOnly, Category = "LockTargetFeedback")
	TObjectPtr<class UWidgetComponent> LockTargetFeedbackWidget;
};
