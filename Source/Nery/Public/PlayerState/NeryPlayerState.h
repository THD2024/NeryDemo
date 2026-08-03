// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "Interface/CombatInterface.h"
#include "NeryPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnAbilityWidgetChangeDelegate);

USTRUCT(Blueprintable,BlueprintType)
struct FAbilityWidgetSlotInfo
{
	GENERATED_BODY();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag;
};


UCLASS()
class NERY_API ANeryPlayerState : public APlayerState,public ICombatInterface
{
	GENERATED_BODY()

public:
	ANeryPlayerState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(ReplicatedUsing = OnRep_InputTagMap, EditDefaultsOnly, Category = "AbilityInfo")
	TArray<FAbilityWidgetSlotInfo> AbilityWidgetSlotInfos;

	FOnAbilityWidgetChangeDelegate OnAbilityWidgetChanged;
	
	UFUNCTION(BlueprintCallable)
	void UpdateAbilityWidgetTag(const FGameplayTag& InInputTag,const FGameplayTag& InTag);
	
	const FGameplayTag GetAbilitySlotTagByInputTag(const FGameplayTag& InInputTag);

protected:

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_InputTagMap();
	
	/*接口*/
	virtual void UpdateAbilityWidgetSlot_Implementation(const FGameplayTag& InInputTag, const FGameplayTag& InTag) override;
	
};
