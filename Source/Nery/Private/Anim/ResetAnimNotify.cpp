// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/ResetAnimNotify.h"
#include"Character/NeryCharacter.h"

void UResetAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (ANeryCharacter* OwnerActor = Cast<ANeryCharacter>(MeshComp->GetOwner()))
	{
		OwnerActor->ResetNotify();
	}
}
