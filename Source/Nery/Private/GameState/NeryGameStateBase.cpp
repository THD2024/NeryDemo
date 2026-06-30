// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/NeryGameStateBase.h"
#include"Net/UnrealNetwork.h"

ANeryGameStateBase::ANeryGameStateBase()
{
	bReplicateUsingRegisteredSubObjectList = true;
}

void ANeryGameStateBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		if (!WidgetTagInfo)
		{
			if (WidgetTagInfoClass)
			{
				WidgetTagInfo = NewObject<UWidgetSlotTagInfo>(this, WidgetTagInfoClass);
				AddReplicatedSubObject(WidgetTagInfo);
			}
			
		}
	}
}

void ANeryGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ANeryGameStateBase, WidgetTagInfo);
}
