// Fill out your copyright notice in the Description page of Project Settings.


#include "TrollAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"


ATrollAIController::ATrollAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SightRadValue = 600.0f;
	PeripheralVisionAngleDegree = 60.0f;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> Obj(TEXT("BehaviorTree'/Game/Blueprints/AI/Troll/AILogic/BT_Troll.BT_Troll'"));
	if (Obj.Succeeded())
	{
		Btree = Obj.Object;
	}
	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	SetupPerceptionSystem();
}

