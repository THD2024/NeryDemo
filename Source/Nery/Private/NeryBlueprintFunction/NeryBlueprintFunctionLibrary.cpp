// Fill out your copyright notice in the Description page of Project Settings.


#include "NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include"Gamemode/NeryGameModeBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include"AbilitySystemComponent.h"
#include"Data/CharacterDataAsset.h"
#include"PlayerState/NeryPlayerState.h"
#include"UI/Controller/AttributeWidgetController.h"
#include"GameState/NeryGameStateBase.h"
#include"UI/HUD/NeryHUD.h"

TSubclassOf<UGameplayEffect> UNeryBlueprintFunctionLibrary::GetCharacterAttackEffect(const UObject* WorldContextObject)
{
	if (ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject))
	{
		if (NeryGameState->CharacterInfo && NeryGameState->CharacterInfo->AttackEffectClass)
		{
			return NeryGameState->CharacterInfo->AttackEffectClass;
		}
	}
	return TSubclassOf<UGameplayEffect>();
}

ANeryGameModeBase* UNeryBlueprintFunctionLibrary::GetGameMode(const UObject* WorldContextObject)
{
	return Cast<ANeryGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

}

ANeryGameStateBase* UNeryBlueprintFunctionLibrary::GetGameState(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// 客户端在启动最初的几帧，这里确实会返回空
		return Cast<ANeryGameStateBase>(World->GetGameState());
	}	
	return nullptr;
}

void UNeryBlueprintFunctionLibrary::InitDefaultAttribute(const UObject* WorldContextObject, AActor* InActor)
{
	//初始化基础属性
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (!bIsEnemy(InActor))
	{
		if (NeryGameState->CharacterInfo && NeryGameState->CharacterInfo->DefaultAttributeEffect)
		{
			ApplyBasicEffectToSelf(InActor, NeryGameState->CharacterInfo->DefaultAttributeEffect);
		}
	}
	else
	{
		if (NeryGameState->EnemyBasicInfo && NeryGameState->EnemyBasicInfo->DefaultAttributeEffect)
		{
			ApplyBasicEffectToSelf(InActor, NeryGameState->EnemyBasicInfo->DefaultAttributeEffect);
		}
	}
	
	
}

void UNeryBlueprintFunctionLibrary::InitSecondaryAttribute(const UObject * WorldContextObject, AActor* InActor)
{
	//初始化二级属性
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (!bIsEnemy(InActor))
	{
		if (NeryGameState->CharacterInfo && NeryGameState->CharacterInfo->SecondaryAttributeEffect)
		{
			ApplyBasicEffectToSelf(InActor, NeryGameState->CharacterInfo->SecondaryAttributeEffect);
		}
	}
	else
	{
		if (NeryGameState->EnemyBasicInfo && NeryGameState->EnemyBasicInfo->SecondaryAttributeEffect)
		{
			ApplyBasicEffectToSelf(InActor, NeryGameState->EnemyBasicInfo->SecondaryAttributeEffect);
		}
	}
}

void UNeryBlueprintFunctionLibrary::InitVitalAttribute(const UObject* WorldContextObject, AActor* InActor)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (!bIsEnemy(InActor))
	{
		if (NeryGameState->CharacterInfo && NeryGameState->CharacterInfo->VitalAttributeEffect)
		{
			ApplyBasicEffectToSelf(InActor, NeryGameState->CharacterInfo->VitalAttributeEffect);
		}
	}
	else
	{
		if (NeryGameState->EnemyBasicInfo && NeryGameState->EnemyBasicInfo->VitalAttributeEffect)
		{
			ApplyBasicEffectToSelf(InActor, NeryGameState->EnemyBasicInfo->VitalAttributeEffect);
		}
	}
	
}

void UNeryBlueprintFunctionLibrary::ApplyBasicEffectToSelf(AActor* InActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass)
{
	UAbilitySystemComponent* InASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
	FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(InGameplayEffectClass, 1, ContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

UAttributeWidgetController* UNeryBlueprintFunctionLibrary::GetAttributeWigetController(const UObject* WorldContextObject, APlayerController* PlayerController)
{//这个扔到蓝图中去
	if (APlayerController* PC = Cast<APlayerController>(PlayerController)) 
	{
		if (ANeryPlayerState* PS = PC->GetPlayerState<ANeryPlayerState>())
		{
			UAbilitySystemComponent* ASC = PS->AbilitySystemComponent;
			UAttributeSet* AS = PS->AttributeSet;
			if (ANeryHUD* NeryHUD = Cast<ANeryHUD>(PC->GetHUD()))
			{
				FWidgetControllerParams Params(PS,PC,ASC,AS);
				UAttributeWidgetController* AttributeWidgetController = NeryHUD->GetAttributeWidgetController(Params);
				AttributeWidgetController->BroadInitValue();
				return AttributeWidgetController;
			}
		}
	}
	return nullptr;

}

bool UNeryBlueprintFunctionLibrary::bIsEnemy(AActor* InActor)
{
	if (InActor->ActorHasTag(FName(TEXT("Enemy"))))
	{
		return true;
	}
	return false;
}
