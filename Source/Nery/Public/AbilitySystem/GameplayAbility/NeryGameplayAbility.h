// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include"GameplayTagContainer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NeryGameplayAbility.generated.h"

/**
 * 
 */


UCLASS()
class NERY_API UNeryGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag DynamicTag;
	
	UFUNCTION(BlueprintCallable,Category = "WarpTarget")
	void MotionWarping(AActor* SelfActor, AActor* TargetActor,FName NotifyName);
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Animation")
	void PlayAnimationSequence(
		UAnimSequenceBase* Sequence,
		float PlayRate = 1.0f);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Animation")
	void OnAnimSequenceFinished();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability|Animation")
	void OnAnimSequenceCancelled();
	
	UFUNCTION(NetMulticast,Reliable)
	void NetMulticast_PlayAnim(USkeletalMeshComponent* Mesh,UAnimSequenceBase* AnimSequenceBase);
	
	UFUNCTION(NetMulticast,Reliable)
	void NetMulticast_RestoreAnimBp();
	
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

private:
	FTimerHandle AnimTimerHandle;

	UPROPERTY()
	TObjectPtr<UAnimSequenceBase> ActiveSequence;

	TSubclassOf<UAnimInstance> OriginalAnim;
	
	
	void RestoreAnimBp();
	void ClearAnimTimer();
};
