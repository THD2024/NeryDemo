// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"AbilitySystemInterface.h"
#include "NeryBaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class ECharacterState: uint8
{
	LockOn,
	Free,
	Attack
};

UCLASS()
class NERY_API ANeryBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANeryBaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//等待子类调用
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
