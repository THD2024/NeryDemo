// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "MotionWarpingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
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
	
	UFUNCTION(BlueprintCallable)
	bool ActivateEnemyAbilityByTag(const FGameplayTag& Tag);
	
	/*行为树*/
	UPROPERTY(EditDefaultsOnly,Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	/*行为树*/
	
	void SetIsLocked(bool InbLocked){ IsLockedOn = InbLocked; };
	
	bool GetIsLocked(){ return IsLockedOn; };
	
protected:

	void BeginPlay() override;
	
	void Tick(float DeltaSeconds) override;

	void PossessedBy(AController* NewController) override;
	
	void GiveEnemyAbilities();

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void OnMaxHealthDelegate(const FOnAttributeChangeData& Data);
	
	void OnMaxPoiseChanged(const FOnAttributeChangeData& Data);
	
	void OnPoiseChanged(const FOnAttributeChangeData& Data);
	
	void UpdatePoiseStatus();
	
	/*接口*/
	virtual void LockTargetFeedBack_Implementation() override;
	virtual void UnLockTargetFeedBack_Implementation()override;
	virtual FTransform GetWeaponLocation_Implementation() override;
	virtual void UpdateWarpTarget_Implementation(FName TargetName, FVector TargetLocation, FRotator TargetRotation) override;
	virtual bool GetEnemyPoiseStatus_Implementation() override;
	virtual void SendEventtoHitReaction_Implementation(const FGameplayTag& InTag, const FGameplayEventData& EventData) override;
	virtual void ActiveHitReaction_Implementation() override;
	virtual void Death_Implementation() override;
	/*接口*/
	
	UPROPERTY(EditDefaultsOnly,Category = "WarpTarget")
	TObjectPtr<UMotionWarpingComponent> WarpingComponent;
	
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
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PoiseRecoverEffect;

	
	FTimerHandle TimerHandle;
	
	bool IsLockedOn = false;
	
	bool bPoiseStatus = true;
	
	bool IsRecovering = false;
	

};
