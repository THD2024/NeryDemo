// Fill out your copyright notice in the Description page of Project Settings.


#include "NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include"Gamemode/NeryGameModeBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include"AbilitySystemComponent.h"
#include"Data/CharacterDataAsset.h"
#include"PlayerState/NeryPlayerState.h"
#include"UI/Controller/AttributeWidgetController.h"
#include"UI/HUD/NeryHUD.h"

TSubclassOf<UGameplayEffect> UNeryBlueprintFunctionLibrary::GetCharacterAttackEffect(const UObject* WorldContextObject)
{
	if (ANeryGameModeBase* NeryGameMode = GetGameMode(WorldContextObject))
	{
		if (NeryGameMode->CharacterInfo && NeryGameMode->CharacterInfo->AttackEffectClass)
		{
			return NeryGameMode->CharacterInfo->AttackEffectClass;
		}
	}
	return TSubclassOf<UGameplayEffect>();
}

ANeryGameModeBase* UNeryBlueprintFunctionLibrary::GetGameMode(const UObject* WorldContextObject)
{
	return CastChecked<ANeryGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	return nullptr;
}

void UNeryBlueprintFunctionLibrary::InitDefaultAttribute(const UObject* WorldContextObject, AActor* InActor)
{
	//初始化基础属性
	ANeryGameModeBase* NeryGameMode = GetGameMode(WorldContextObject);
	if (NeryGameMode->CharacterInfo && NeryGameMode->CharacterInfo->DefaultAttributeEffect)
	{
		UAbilitySystemComponent* InASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
		FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(NeryGameMode->CharacterInfo->DefaultAttributeEffect, 1, ContextHandle);
		InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	
	
}

void UNeryBlueprintFunctionLibrary::InitSecondaryAttribute(const UObject * WorldContextObject, AActor* InActor)
{
	//初始化二级属性
	ANeryGameModeBase* NeryGameMode = GetGameMode(WorldContextObject);
	if (NeryGameMode->CharacterInfo && NeryGameMode->CharacterInfo->SecondaryAttributeEffect)
	{
		UAbilitySystemComponent* InASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
		FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(NeryGameMode->CharacterInfo->SecondaryAttributeEffect, 1, ContextHandle);
		InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UNeryBlueprintFunctionLibrary::InitVitalAttribute(const UObject* WorldContextObject, AActor* InActor)
{
	ANeryGameModeBase* NeryGameMode = GetGameMode(WorldContextObject);
	if (NeryGameMode->CharacterInfo && NeryGameMode->CharacterInfo->VitalAttributeEffect)
	{
		UAbilitySystemComponent* InASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
		FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(NeryGameMode->CharacterInfo->VitalAttributeEffect, 1, ContextHandle);
		InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
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
