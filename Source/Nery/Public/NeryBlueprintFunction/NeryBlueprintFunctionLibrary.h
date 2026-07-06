// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NeryBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
class UGameplayEffect;
class ANeryGameModeBase;
class ANeryGameStateBase;
class UAttributeWidgetController;
class UCharacterDataAsset;
struct FBuffNumberInfo;
class UItemBagDataAsset;
class UWidgetSlotTagInfo;

UCLASS()
class NERY_API UNeryBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	//自动找出玩家基础信息中的普攻游戏效果，该函数目前仅限玩家,后面会扩展到适配敌人的data
	static TSubclassOf<UGameplayEffect> GetCharacterAttackEffect(const UObject* WorldContextObject);

	UFUNCTION()
	static ANeryGameModeBase* GetGameMode(const UObject* WorldContextObject);

	UFUNCTION()
	static ANeryGameStateBase* GetGameState(const UObject* WorldContextObject);

	UFUNCTION()
	static void InitDefaultAttribute(const UObject* WorldContextObject, AActor* InActor);

	UFUNCTION()
	static void InitSecondaryAttribute(const UObject* WorldContextObject, AActor* InActor);

	UFUNCTION()
	static void InitVitalAttribute(const UObject* WorldContextObject, AActor* InActor);

	static void ApplyBasicEffectToSelf(AActor* InActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass);

	UFUNCTION(BlueprintPure,BlueprintCallable)
	static UAttributeWidgetController* GetAttributeWigetController(const UObject* WorldContextObject, APlayerController* PlayerController);

	UFUNCTION(BlueprintPure,BlueprintCallable)
	static bool bIsEnemy(AActor* InActor);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static UItemBagDataAsset* GetItemBagFromActor(AActor* InActor);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static const FGameplayTag GetCurrentBuffWidgetTag(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void SetCurrentBuffWidgetTag(const UObject* WorldContextObject,const FGameplayTag& InTag);

	UFUNCTION(BlueprintPure,BlueprintCallable)
	static int32 GetLevel(const UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static float GetXpByLevel(const UObject* WorldContextObject, float InLevel);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static float GetAttributePointbyCurrentLevel(const UObject* WorldContextObject, float CurrentLevel);
};
