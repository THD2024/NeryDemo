// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryBaseCharacter.h"
#include"EffectActor/Weapon.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"
#include"Character/NeryCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ANeryBaseCharacter::ANeryBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNeryCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}



// Called when the game starts or when spawned
void ANeryBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ANeryBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ANeryBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANeryBaseCharacter, Weapon);
}

void ANeryBaseCharacter::AllowAttack_Implementation()
{
	if (Weapon)
	{
		Weapon->IgnoreActors.Empty();
		Weapon->SetCanWeaponTrace(true);
	}
}

void ANeryBaseCharacter::CloseAttack_Implementation()
{
	if (Weapon)
	{
		Weapon->SetCanWeaponTrace(false);
		Weapon->IgnoreActors.Empty();
	}
}

void ANeryBaseCharacter::BegintoTrace_Implementation()
{
	if (Weapon)
	{
		Weapon->WeaponTrace();
	}
}

FTransform ANeryBaseCharacter::GetWeaponLocation_Implementation()
{
	return FTransform();
}

FGenericTeamId ANeryBaseCharacter::GetGenericTeamId() const
{
	return TeamId;
}

void ANeryBaseCharacter::Death_Implementation()
{
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->CancelAllAbilities();
		GetAbilitySystemComponent()->RemoveReplicatedLooseGameplayTag(FNeryGameplayTags::GetNeryGameplayTags().Status_Energy_Full);
		GetAbilitySystemComponent()->RemoveReplicatedLooseGameplayTag(FNeryGameplayTags::GetNeryGameplayTags().Status_Rolling);
	}
	Multicast_HandleDeath();
}


void ANeryBaseCharacter::InitAttribute()
{
	if (bInitialized == true)return;
	UNeryBlueprintFunctionLibrary::InitDefaultAttribute(this, this);
	UNeryBlueprintFunctionLibrary::InitSecondaryAttribute(this, this);
	UNeryBlueprintFunctionLibrary::InitVitalAttribute(this, this);
	bInitialized = true;
}

void ANeryBaseCharacter::SpawnWeapon()
{
	if (WeaponClass)
	{//在生成actor是，owner和都已经设置。
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		Weapon = GetWorld()->SpawnActor<AWeapon>
			(
				WeaponClass,
				GetActorLocation(),
				GetActorRotation(),
				SpawnParameters
			);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		Weapon->SetOwner(this);
	}
}

// Called every frame
void ANeryBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANeryBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANeryBaseCharacter::Multicast_HandleDeath_Implementation()
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//布娃娃表现
	GetMesh()->SetCollisionProfileName(FName("Ragdoll")); // 使用内置 Ragdoll 配置
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
 
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

