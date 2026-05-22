// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include"AbilitySystemComponent.h"//获取属性值时需要用到的头文件,被通知和注册属性复制时也需要用到
#include "NeryAttributeSet.generated.h"


//内置的get set函数宏，分别是获取属性的属性，获取属性值，设置属性值，初始化属性值
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)	

/**
 * 
 */
UCLASS()
class NERY_API UNeryAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UNeryAttributeSet();

	//属性访问器宏，传入类名和属性名
	ATTRIBUTE_ACCESSORS(UNeryAttributeSet, Health);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	/*Delegate Call back Function*/

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

};
