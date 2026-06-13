// Fill out your copyright notice in the Description page of Project Settings.


#include "NeryAssetManager.h"
#include"AbilitySystem/NeryGameplayTag.h"
#include"AbilitySystemGlobals.h"

UNeryAssetManager& UNeryAssetManager::Get()
{
	// TODO: 在此处插入 return 语句
	check(GEngine);
	UNeryAssetManager* NeryAssetManager = Cast<UNeryAssetManager>(GEngine->AssetManager);
	return *NeryAssetManager;
}

void UNeryAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	//这里用来记载游戏基本数据，所以标签放在这里初始化
	FNeryGameplayTags::InitializeTags();
	//这里因为assetmanager需要调用全局初始化函数。
	UAbilitySystemGlobals::Get().InitGlobalData();

}
