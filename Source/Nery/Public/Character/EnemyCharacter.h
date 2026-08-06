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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyMaxHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyMaxPoiseDelegate, float, MaxPoise);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyPoiseDelegate, float, Poise);

UCLASS()
class NERY_API AEnemyCharacter : public ANeryBaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	void BindCallbacks();

	UFUNCTION(BlueprintImplementableEvent)
	void InitWidget();

protected:

	void BeginPlay() override;

	void PossessedBy(AController* NewController) override;
	
	void GiveEnemyAbilities();

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void OnMaxHealthDelegate(const FOnAttributeChangeData& Data);
	
	void OnMaxPoiseChanged(const FOnAttributeChangeData& Data);
	
	void OnPoiseChanged(const FOnAttributeChangeData& Data);

	virtual void LockTargetFeedBack_Implementation() override;
	virtual void UnLockTargetFeedBack_Implementation()override;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyHealthChanged OnEnemyHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyMaxHealth MaxHealthDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnEnemyMaxPoiseDelegate MaxPoiseDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnEnemyPoiseDelegate PoiseDelegate;
	
	UPROPERTY(EditDefaultsOnly, Category = "LockTargetFeedback")
	TObjectPtr<class UWidgetComponent> LockTargetFeedbackWidget;
	
	UPROPERTY(EditAnywhere,Category = "Ability")
	TArray<FGameplayTag> EnemyAbilities;//敌人拥有的能力
};
