// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NeryBaseCharacter.h"
#include "NeryCharacter.generated.h"

class ANeryPlayerState;
class UActorComponent;

/**
 * 
 */
UCLASS()
class NERY_API ANeryCharacter : public ANeryBaseCharacter
{
	GENERATED_BODY()

public:
	ANeryCharacter();

	void InitHUD();

	void SetMaxWalkSpeed(float NewMaxWalkSpeed);

	UFUNCTION(BlueprintImplementableEvent)
	void LinkAnimTiming(bool IsLockOn);

	UFUNCTION(BlueprintCallable)
	bool GetIsLockOn();

	UFUNCTION(BlueprintCallable)
	AActor* GetLockOnTarget();

	UFUNCTION(BlueprintCallable)
	void SaveNotify();

	UFUNCTION(BlueprintCallable)
	void ResetNotify();

	void ReceiveAttackInput();

	void HandleAttackLogic();

	void UseBuffActor();

	UPROPERTY(ReplicatedUsing = OnRep_LockOn)
	bool bIsLockOn_NetWorked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RunMaxWalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RunNormalWalkSpeed = 450.f;

	UPROPERTY(Replicated,VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterAttackState AttackState = ECharacterAttackState::None;//在none状态下，可以播放攻击动画

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Data")
	TObjectPtr<class UCharacterDataAsset> CharacterDataAsset;//通过DataAsset存储角色的攻击动画，受击动画，Ability等数据，将角色属性和本身的表现分离开来，方便后续的调整和扩展

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<class UItemBagDataAsset> ItemBag;

	

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SmoothRotateToTarget(AActor* TargetActor, float DeltaTime);

	void SetLockMode(bool bIsLockOn);

	void PossessedBy(AController* NewController) override;

	void OnRep_PlayerState() override;
	
	void InitASCandAttribute();



	/*网络复制*/

	UPROPERTY(Replicated)
	TObjectPtr<AActor>NetLockedTarget;

	UFUNCTION(Server, Reliable)
	void Server_SetLockTarget(AActor* NewTarget);

	UFUNCTION(Server, Reliable)
	void Server_SetMaxWalkSpeed(float NewMaxWalkSpeed);

	UFUNCTION(Server, Reliable)
	void Server_ReceiveAttackInput();

	UFUNCTION(Server, Reliable)
	void Server_SetLockStatus(bool bIsLockOn);//将锁定状态通知到服务器，然后服务器分发到客户端

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackMontage(const int32 Index);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_LockOn();

	UFUNCTION(Server, Unreliable)
	void Server_UpdateRotation(float DeltaTime);

	/*网络复制*/

	/*Interface*/
	virtual ECharacterAttackState GetAttackState_Implementation() override;

	/*Interface*/


	float RotateSpeed = 10.f;//角色旋转的速度

	int32 ClickTime = 0;//表示当前的攻击次数

	bool bAnimNotified = false;
	bool bInputBuffered = false;//这个变量用来表示当前是否有输入被缓冲了，如果有输入被缓冲了，就说明在当前攻击动画播放的过程中，玩家又按了一次攻击输入，这时就可以在动画蒙太奇的Notify节点中通过判断这个变量来播放下一个攻击动画，实现连续攻击的逻辑
	
};
