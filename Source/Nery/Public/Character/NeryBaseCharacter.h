// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"AbilitySystemInterface.h"
#include"NeryType.h"
#include "Net/UnrealNetwork.h"
#include"Interface/CombatInterface.h"
#include "NeryBaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UStaticMeshComponent;
class AWeapon;


UCLASS()
class NERY_API ANeryBaseCharacter : public ACharacter, public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANeryBaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(Replicated,EditAnywhere, Category = "Weapon")
	TObjectPtr<AWeapon> Weapon;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//等待子类调用
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitAttribute();

	void SpawnWeapon();

	bool bInitialized = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
