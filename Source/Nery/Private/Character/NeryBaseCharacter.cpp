// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryBaseCharacter.h"
#include"EffectActor/Weapon.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"


// Sets default values
ANeryBaseCharacter::ANeryBaseCharacter()
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
	{//在生成actor是，owner和owningactor都已经设置。
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
		Weapon->SetOwningActor(this);
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

