// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor/Weapon.h"
#include"Components/BoxComponent.h"
#include"Components/SceneComponent.h"
#include"Interface/CombatInterface.h"
#include"Kismet/KismetSystemLibrary.h"
#include"NeryType.h"
#include"GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include"GameplayTagContainer.h"
#include "Sound/SoundBase.h"
#include"AbilitySystemComponent.h"
#include"Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include"NeryBlueprintFunction/NeryBlueprintFunctionLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	PivotRoot = CreateDefaultSubobject<USceneComponent>("Pivot");
	TraceBox = CreateDefaultSubobject<UBoxComponent>("TraceBox");
	TraceStart = CreateDefaultSubobject<USceneComponent>("TraceStart");
	TraceEnd = CreateDefaultSubobject<USceneComponent>("TraceEnd");
	WeaponShape = CreateDefaultSubobject<UStaticMeshComponent>("WeaponShape");
	SetRootComponent(PivotRoot);
	TraceBox->SetupAttachment(RootComponent);
	TraceStart->SetupAttachment(RootComponent);
	TraceEnd->SetupAttachment(RootComponent);
	WeaponShape->SetupAttachment(RootComponent);
	TraceBox->SetCollisionObjectType(ECC_GameTraceChannel1);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	TraceBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
	TraceBox->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnBoxEndOverlap);
}

void AWeapon::ApplyAttackEffect(AActor* TargetActor,const FHitResult& HitResult)
{
	//在这里面apply游戏效果
	
	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner))
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
		{
			FGameplayEffectContextHandle ContextHandle = OwnerASC->MakeEffectContext();
			ContextHandle.AddHitResult(HitResult);
			ContextHandle.AddInstigator(Owner, this);
			int32 CurrentLevel = UNeryBlueprintFunctionLibrary::GetLevel(OwnerASC);
			FGameplayEffectSpecHandle SpecHandle = OwnerASC->MakeOutgoingSpec(UNeryBlueprintFunctionLibrary::GetCharacterAttackEffect(Owner), CurrentLevel, ContextHandle);
			float DamageValue = UNeryBlueprintFunctionLibrary::GetNormalDamageByLevel(GetWorld(), CurrentLevel);
			FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag("Damage.Normal");
			SpecHandle.Data->SetSetByCallerMagnitude(DamageTag, DamageValue);
			OwnerASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}
	}
}


void AWeapon::BoxTrace(ECollisionChannel DetectiveObjectType)
{
	//在这里进行盒型检测
	TArray<FHitResult> HitResults;
	const FVector TraceStartLocation = TraceStart->GetComponentLocation();
	const FVector TraceEndLocation = TraceEnd->GetComponentLocation();
	ETraceTypeQuery TraceChannel;
	TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	if (GetOwner())
	{
		IgnoreActors.AddUnique(Owner);
	}
	bool bHit = UKismetSystemLibrary::BoxTraceMulti
	(
		this,
		TraceStartLocation,
		TraceEndLocation,
		FVector(5.f, 5.f, 5.f),
		TraceBox->GetComponentRotation(),
		TraceChannel,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);
	if (bHit)
	{
		for (const auto& HitResult : HitResults)
		{
			if (!IgnoreActors.Contains(HitResult.GetActor()))
			{
				ApplyAttackEffect(HitResult.GetActor(),HitResult);//在确定检测有效后，尝试添加游戏效果
			}
			IgnoreActors.AddUnique(HitResult.GetActor());

		}
	}
}

void AWeapon::WeaponTrace()
{
	BoxTrace(ECC_Pawn);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanWeaponTrace)
	{
		WeaponTrace();
	}
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (!GetOwner()->Implements<UCombatInterface>())return;
	// if (ICombatInterface::Execute_GetAttackState(GetOwner()) == ECharacterAttackState::Attacking)
	// {
	
		CanWeaponTrace = true;
	// }
	// else
	// {
	// 	CanWeaponTrace = false;
	// }
}

void AWeapon::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CanWeaponTrace = false;
}

