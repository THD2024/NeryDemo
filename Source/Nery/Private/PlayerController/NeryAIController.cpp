// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/NeryAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include"Character/EnemyCharacter.h"

ANeryAIController::ANeryAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseConfig"));
	
	//配置感知数据
	SenseConfig->SightRadius = AISightRadius;//检测方位
	SenseConfig->LoseSightRadius = AILoseSightRadius;//超出这个距离，敌人不在追击
	SenseConfig->PeripheralVisionAngleDegrees = VisionDegree;//视角半角，总范围视角就是120度
	SenseConfig->SetMaxAge(AIMaxAge);//设置记忆时间
	
	//设置感知
	SenseConfig->DetectionByAffiliation.bDetectEnemies = true;
	SenseConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SenseConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	//将配置注册到感官组件中
	PerceptionComponent->ConfigureSense(*SenseConfig);
	PerceptionComponent->SetDominantSense(SenseConfig->GetSenseImplementation());
}

void ANeryAIController::BeginPlay()
{
	Super::BeginPlay();
	//该函数是用来处理单个感应对象的，与之对应的是onperceptionupdated，会返回一个感应对象数据.
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANeryAIController::OnTargetPerceptionUpdated);
}

void ANeryAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);//在控制器附身成功时，激活行为树.
	if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn))
	{
		if (EnemyCharacter->BehaviorTree)
		{
			RunBehaviorTree(EnemyCharacter->BehaviorTree);
		}
	}
}

void ANeryAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	//这里暂时先手动编码，保证是刚看到，就直接走向actor，后面通过这里为入口来实现行为树。
	if (Stimulus.WasSuccessfullySensed() == true)//表示当前是否是刚好看到
	{
		if (Blackboard->GetValueAsObject(FName("Player")) == Actor)return;//这里保证一直锁定到角色
		Blackboard->SetValueAsObject(FName("Player"),Actor);
	}
}
