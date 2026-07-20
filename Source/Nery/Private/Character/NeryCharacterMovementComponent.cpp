// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NeryCharacterMovementComponent.h"
#include"GameFramework/Character.h"
#include "Settings/LevelEditorPlayNetworkEmulationSettings.h"

void UNeryCharacterMovementComponent::ProcessClientTimeStampForTimeDiscrepancy(float ClientTimeStamp,
                                                                               FNetworkPredictionData_Server_Character& ServerData)
{
	//在两端时间偏差过大的时候，通过手动将时间偏差值给缩小，防止出现服务器暴力拉回或者追赶的问题
	float CurrentDiscrepancy = 0.0f;
	CurrentDiscrepancy = ServerData.TimeDiscrepancy;
	if (FMath::Abs(CurrentDiscrepancy) > 0.1)//当偏差大于0.1s
	{
		float OldDiscrepancy = ServerData.TimeDiscrepancy;
		ServerData.TimeDiscrepancy = FMath::Clamp(OldDiscrepancy,0.f,0.05f);//手动将时间偏差设置到一个合适的范围内
		Super::ProcessClientTimeStampForTimeDiscrepancy(ClientTimeStamp, ServerData);

	}
	else
	{//正常情况下就直接调用正常的父类函数进行处理
		Super::ProcessClientTimeStampForTimeDiscrepancy(ClientTimeStamp, ServerData);
	}
}


