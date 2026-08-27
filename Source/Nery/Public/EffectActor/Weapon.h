// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBoxComponent;
class USceneComponent;
class UStaticMeshComponent;
class UGameplayEffect;
class USoundBase;


UCLASS()
class NERY_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USceneComponent> ScenePoint;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	void ApplyAttackEffect(AActor* TargetActor,const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly, Category = "WeaponCollision")
	TObjectPtr<USceneComponent> PivotRoot;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponCollision")
	TObjectPtr<UBoxComponent> TraceBox;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponCollision")
	TObjectPtr<USceneComponent> TraceStart;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponCollision")
	TObjectPtr<USceneComponent> TraceEnd;

	UPROPERTY(EditDefaultsOnly, Category = "WeaponCollision")
	TObjectPtr<UStaticMeshComponent> WeaponShape;

	UPROPERTY(EditDefaultsOnly,Category = "Weapon")
	TSubclassOf<UGameplayEffect> EnergyEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "AttackSound")
	TObjectPtr<USoundBase> AttackSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void BoxTrace(ECollisionChannel DetectiveObjectType);
	
	void WeaponTrace();
	
	void SetCanWeaponTrace(const bool Inbool);
	
	UPROPERTY(ReplicatedUsing = OnRep_OnCanWeaponTrace)
	bool CanWeaponTrace = false;
	
	
	//设置新的武器是注意这个忽略数组
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> IgnoreActors;

	
	/*网络复制*/
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_OnCanWeaponTrace();
	
	UFUNCTION(Server,Reliable)
	void Server_ApplyDamage(AActor* TargetActor,const FHitResult& HitResult);
};
