// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/NeryAbilityDataAsset.h"
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
class UAbilityWidgetController;

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
	
	//改函数可以选择填充fhitresult对需要通过游戏效果出发gameplaycue的情况友好
	static void ApplyEffectToActor(AActor* InActor,TSubclassOf<UGameplayEffect> InGameplayEffectClass,const FHitResult& HitResult = FHitResult());
	
	static void ApplyEffectToTarget(AActor* Instigator,AActor* TargetActor,TSubclassOf<UGameplayEffect>InGameplayEffectClass,const FHitResult& HitResult = FHitResult());
	
	UFUNCTION(BlueprintPure,BlueprintCallable)
	static UAttributeWidgetController* GetAttributeWigetController(const UObject* WorldContextObject, APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable,BlueprintCallable)
	static UAbilityWidgetController* GetAbilityWidgetController(const UObject* WorldContextObject,APlayerController* PlayerController);
	
	UFUNCTION(BlueprintPure,BlueprintCallable)
	static bool bIsEnemy(AActor* InActor);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static UItemBagDataAsset* GetItemBagFromActor(AActor* InActor);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	static const FGameplayTag GetCurrentBuffWidgetTag(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void SetCurrentBuffWidgetTag(const UObject* WorldContextObject,const FGameplayTag& InTag);

	UFUNCTION(BlueprintPure,BlueprintCallable)//需要获取玩家等级调用这个函数
	static int32 GetLevel(const UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintPure, BlueprintCallable)//计算等级的辅助函数,仅限于attributeset
	static float GetXpByLevel(const UObject* WorldContextObject, float InLevel);

	UFUNCTION(BlueprintPure, BlueprintCallable)//仅限于attributeset
	static float GetAttributePointbyCurrentLevel(const UObject* WorldContextObject, float CurrentLevel);

	UFUNCTION(BlueprintPure, BlueprintCallable)//此处通过获取显示到视口的等级来获取普通伤害，这里是int32类型。
	static float GetNormalDamageByLevel(const UObject* WorldContextObject, int32 CurrentLevel);

	UFUNCTION(BlueprintCallable)
	static void AddBasicAttributePoints(const UObject* WorldContextObject, const FGameplayTag& AttributeTag, AActor* InActor);

	UFUNCTION(BlueprintCallable)
	static void SetMagicSlotAbilitySlotTag(const UObject* WorldContextObject,const FGameplayTag& InAbilitySlotTag);
	
	UFUNCTION(BlueprintCallable)
	static void SetPhysicalAbilitySlotTag(const UObject* WorldContextObject,const FGameplayTag& InputTag,const FGameplayTag& InAbilitySlotTag);
	
	UFUNCTION(BlueprintPure,BlueprintCallable)
	static FGameplayTag GetCurrentAbilitySlotTagByInputTag(const UObject* WorldContextObject,const FGameplayTag& InputTag);
	
	static void ApplyEffectToSelfBySetByCaller(AActor* InActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass, const FGameplayTag& AttributeTag);//注意，如果是用在除了属性加点之外的，那么需要注意这里的setbycaller设置改为从曲线表来获取数据，更加灵活。

	static UWidgetSlotTagInfo* GetWidgeetSlotTagInfo(const UObject* WorldContextObject);
	
	static UNeryAbilityDataAsset* GetAbilityDataAsset(const UObject* WorldContextObject);
	
	static UAbilitySystemComponent* GetAbilitySystemComponentByActor(AActor* InActor);
};
