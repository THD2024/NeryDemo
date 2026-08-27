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
#include"Interface/CombatInterface.h"
#include"Data/WidgetSlotTagInfo.h"
#include"AbilitySystem/NeryAttributeSet.h"
#include "Character/EnemyCharacter.h"
#include "Character/NeryCharacter.h"
#include"UI/Controller/AbilityWidgetController.h"
#include"UI/Widget/NeryUserWidget.h"

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
	if (!InASC)return;
	FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(InGameplayEffectClass, GetLevel(InASC), ContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UNeryBlueprintFunctionLibrary::ApplyEffectToActor(AActor* InActor,
	TSubclassOf<UGameplayEffect> InGameplayEffectClass, const FHitResult& HitResult)
{
	UAbilitySystemComponent* InASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
	if (!InASC)return;
	FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
	ContextHandle.AddHitResult(HitResult);
	FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(InGameplayEffectClass, GetLevel(InASC), ContextHandle);
	InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
}

void UNeryBlueprintFunctionLibrary::ApplyEffectToTarget(AActor* Instigator, AActor* TargetActor,
	TSubclassOf<UGameplayEffect> InGameplayEffectClass, const FHitResult& HitResult,bool CanSendEvent)
{
	UAbilitySystemComponent* InstigatorASC = GetAbilitySystemComponentByActor(Instigator);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!Instigator || !TargetASC)return;
	FGameplayEffectContextHandle ContextHandle = InstigatorASC->MakeEffectContext();
	ContextHandle.AddHitResult(HitResult);
	FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(InGameplayEffectClass,GetLevel(InstigatorASC), ContextHandle);
	InstigatorASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),TargetASC);
	
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

UAbilityWidgetController* UNeryBlueprintFunctionLibrary::GetAbilityWidgetController(const UObject* WorldContextObject,
	APlayerController* PlayerController)
{
	if (APlayerController* PC = Cast<APlayerController>(PlayerController)) 
	{
		if (ANeryPlayerState* PS = PC->GetPlayerState<ANeryPlayerState>())
		{
			UAbilitySystemComponent* ASC = PS->AbilitySystemComponent;
			UAttributeSet* AS = PS->AttributeSet;
			if (ANeryHUD* NeryHUD = Cast<ANeryHUD>(PC->GetHUD()))
			{
				FWidgetControllerParams Params(PS,PC,ASC,AS);
				UAbilityWidgetController* AbilityWidgetController = NeryHUD->GetAbilityWidgetController(Params);
				if (NeryHUD->GetOverlayWidget()->Implements<UCombatInterface>())
				{
					ICombatInterface::Execute_OnAbilityWidgetControllerSet(NeryHUD->GetOverlayWidget(),AbilityWidgetController);
				}
				AbilityWidgetController->BroadInitValue();
				return AbilityWidgetController;
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

UItemBagDataAsset* UNeryBlueprintFunctionLibrary::GetItemBagFromActor(AActor* InActor)
{
	if (InActor && InActor->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetItemBag(InActor);
	}
	return nullptr;
}

const FGameplayTag UNeryBlueprintFunctionLibrary::GetCurrentBuffWidgetTag(const UObject* WorldContextObject)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->WidgetTagInfo)
	{
		return NeryGameState->WidgetTagInfo->GetBuffWidgetTag();
	}
	return FGameplayTag();
}

void UNeryBlueprintFunctionLibrary::SetCurrentBuffWidgetTag(const UObject* WorldContextObject, const FGameplayTag& InTag)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->WidgetTagInfo)
	{
		 NeryGameState->WidgetTagInfo->SetBuffActorWidgetTagByTag(InTag);
	}
}

int32 UNeryBlueprintFunctionLibrary::GetLevel(const UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent && !AbilitySystemComponent->GetAvatarActor()->ActorHasTag(FName("Enemy")))
	{
		if (const UNeryAttributeSet* AttributeSet = Cast<UNeryAttributeSet>(AbilitySystemComponent->GetAttributeSet(UAttributeSet::StaticClass())))
		{
			return FMath::FloorToInt(AttributeSet->GetLevel());//向下取整
		}
	}
	return 1;
}

float UNeryBlueprintFunctionLibrary::GetXpByLevel(const UObject* WorldContextObject, const float InLevel)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->LevelUpInfo)
	{
		return NeryGameState->LevelUpInfo->GetXPFromLevel(InLevel);
	}
	return 0.0f;
}

float UNeryBlueprintFunctionLibrary::GetAttributePointbyCurrentLevel(const UObject* WorldContextObject, const float CurrentLevel)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->LevelUpInfo)
	{
		return NeryGameState->LevelUpInfo->GetAttributePointsFromLevel(CurrentLevel);
	}
	return 0.0f;
}

float UNeryBlueprintFunctionLibrary::GetNormalDamageByLevel(const UObject* WorldContextObject, const int32 CurrentLevel)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->LevelUpInfo)
	{
		return NeryGameState->LevelUpInfo->GetNormalDamageFromLevel(CurrentLevel);
	}
	return 0.0f;
}

void UNeryBlueprintFunctionLibrary::AddBasicAttributePoints(const UObject* WorldContextObject, const FGameplayTag& AttributeTag,  AActor* InActor)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->LevelUpInfo)
	{
		if(NeryGameState->LevelUpInfo->GetUpgradeBasicAttributeEffect())
		{
			// Handle the attribute point addition logic here
			ApplyEffectToSelfBySetByCaller(InActor, NeryGameState->LevelUpInfo->GetUpgradeBasicAttributeEffect(), AttributeTag);
		}
	}
}

void UNeryBlueprintFunctionLibrary::SetMagicSlotAbilitySlotTag(const UObject* WorldContextObject,
	const FGameplayTag& InAbilitySlotTag)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->WidgetTagInfo)
	{
		NeryGameState->WidgetTagInfo->SetMagicAbilitySlotTag(InAbilitySlotTag);
	}
}

void UNeryBlueprintFunctionLibrary::SetPhysicalAbilitySlotTag(const UObject* WorldContextObject,
	const FGameplayTag& InputTag, const FGameplayTag& InAbilitySlotTag)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->WidgetTagInfo)
	{
		NeryGameState->WidgetTagInfo->SetPhysicalAbilitySlotTag(InputTag, InAbilitySlotTag);
	}
}

FGameplayTag UNeryBlueprintFunctionLibrary::GetCurrentAbilitySlotTagByInputTag(const UObject* WorldContextObject,
                                                                               const FGameplayTag& InputTag)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->WidgetTagInfo)
	{
		NeryGameState->WidgetTagInfo->GetAbilitySlotTagByInputTag(InputTag);
	}
	return FGameplayTag();
}


void UNeryBlueprintFunctionLibrary::ApplyEffectToSelfBySetByCaller(AActor* InActor, TSubclassOf<UGameplayEffect> InGameplayEffectClass, const FGameplayTag& AttributeTag)
{
	UAbilitySystemComponent* InASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
	if (!InASC)return;
	FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(InGameplayEffectClass, 1, ContextHandle);
	SpecHandle.Data->SetSetByCallerMagnitude(AttributeTag, 1.f);
	InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

UWidgetSlotTagInfo* UNeryBlueprintFunctionLibrary::GetWidgeetSlotTagInfo(const UObject* WorldContextObject)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->WidgetTagInfo)
	{
		return NeryGameState->WidgetTagInfo;
	}
	return nullptr;
}

UNeryAbilityDataAsset* UNeryBlueprintFunctionLibrary::GetAbilityDataAsset(const UObject* WorldContextObject)
{
	ANeryGameStateBase* NeryGameState = GetGameState(WorldContextObject);
	if (NeryGameState->AbilityDataAsset)
	{
		return NeryGameState->AbilityDataAsset;
	}
	return nullptr;
}

UAbilitySystemComponent* UNeryBlueprintFunctionLibrary::GetAbilitySystemComponentByActor(AActor* InActor)
{
	if (!InActor)return nullptr;
	if (ANeryCharacter* NeryCH = Cast<ANeryCharacter>(InActor))
	{
		return NeryCH->AbilitySystemComponent;
	}
	if (AEnemyCharacter* EmCH = Cast<AEnemyCharacter>(InActor))
	{
		return EmCH->AbilitySystemComponent;
	}
	return nullptr;
}

void UNeryBlueprintFunctionLibrary::ApplyEffectToActor(AActor* InActor, TSubclassOf<UGameplayEffect>InGameplayEffectClass,
	const FHitResult& HitResult, const FGameplayTag& SetByCallerTag,bool CanSendEvent)
{
	UAbilitySystemComponent* InASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor);
	if (!InASC)return;
	FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
	ContextHandle.AddHitResult(HitResult);
	float DamageValue = UNeryBlueprintFunctionLibrary::GetNormalDamageByLevel(InActor,GetLevel(InASC));
	FGameplayEffectSpecHandle SpecHandle = InASC->MakeOutgoingSpec(InGameplayEffectClass, 1, ContextHandle);
	SpecHandle.Data->SetSetByCallerMagnitude(SetByCallerTag, DamageValue);
	InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	// if (CanSendEvent)
	// {
	// 	FGameplayEventData EventData;
	// 	EventData.ContextHandle = ContextHandle;
	// 	FGameplayTag EventTag = FNeryGameplayTags::GetNeryGameplayTags().Event_EventData;
	// 	HandleGameplayEvent(InASC,EventData,EventTag);
	// }
	
}

void UNeryBlueprintFunctionLibrary::HandleGameplayEvent(UAbilitySystemComponent* InASC,
	const FGameplayEventData& InPayLoad,const FGameplayTag& EventTag)
{
	if (!InASC)return;
	InASC->HandleGameplayEvent(EventTag,&InPayLoad);
}

bool UNeryBlueprintFunctionLibrary::IsHitFromFront(const FHitResult& HitResult, AActor* InActor)
{
	if (InActor)
	{
		FVector ActorLocation = InActor->GetActorLocation();
		FVector ActorForwardVector = InActor->GetActorForwardVector();
		FVector HitLocation = HitResult.ImpactPoint;
		FVector DirectiontoHit = (HitLocation - ActorLocation).GetSafeNormal();
		float DotProduct = FVector::DotProduct(DirectiontoHit,DirectiontoHit);
		if (DotProduct > 0.0f)
		{
			return true;
		}
	}
	return false;
}

int32 UNeryBlueprintFunctionLibrary::GetHitDirection(const FHitResult& HitResult, AActor* InActor)
{
	if (InActor)
	{
		FVector ActorLocation = InActor->GetActorLocation();
		FVector HitLocation = HitResult.ImpactPoint;
		FVector DirectiontoHit = (HitLocation - ActorLocation).GetSafeNormal();
		FVector LocalHitVector = InActor->GetActorRotation().UnrotateVector(DirectiontoHit);
		if (LocalHitVector.X > 0.5f)
		{
			return 1;
		}
		if (LocalHitVector.X < -0.5f)
		{
			return 2;
		}
		if (LocalHitVector.Y > 0.5f)
		{
			return 3;
		}
		if (LocalHitVector.Y < -0.5f)
		{
			return 4;
		}
	}
	return 0;
}



