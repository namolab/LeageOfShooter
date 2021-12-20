// Fill out your copyright notice in the Description page of Project Settings.


#include "KrakenAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"


AKrakenAIController::AKrakenAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SightRadValue = 1000.0f;
	PeripheralVisionAngleDegree = 60.0f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> Obj(TEXT("BehaviorTree'/Game/Blueprints/AI/Kraken/AILogic/BT_Kraken.BT_Kraken'"));
	
	if (Obj.Succeeded())
	{
		Btree = Obj.Object;
	}

	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	SetupPerceptionSystem();
}