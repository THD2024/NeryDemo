// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryBaseCharacter.h"

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

