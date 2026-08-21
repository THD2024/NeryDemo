// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include"NeryType.h"
#include "Net/UnrealNetwork.h"
#include"Interface/CombatInterface.h"
#include"AbilitySystem/NeryGameplayTag.h"
#include "NeryBaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UStaticMeshComponent;
class AWeapon;


UCLASS()
class NERY_API ANeryBaseCharacter : public ACharacter, public IAbilitySystemInterface,public ICombatInterface,public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANeryBaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AWeapon> Weapon;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//等待子类调用
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AllowAttack_Implementation() override;
	virtual void CloseAttack_Implementation() override;
	virtual void BegintoTrace_Implementation() override;
	virtual FTransform GetWeaponLocation_Implementation() override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AI")
	FGenericTeamId TeamId = FGenericTeamId::NoTeam;//默认值，子类自己单独设置
	
	void InitAttribute();

	void SpawnWeapon();

	bool bInitialized = false;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
