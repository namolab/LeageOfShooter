// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer_BTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "LeageOfShooter/Character/AI/Controller/BaseAIController.h"
#include "LeageOfShooter/Character/AI/BlackboardKey.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"


UChasePlayer_BTask::UChasePlayer_BTask()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer_BTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseAIController* const Cont = Cast<ABaseAIController>(OwnerComp.GetAIOwner()))
	{
		FVector const PlayerLocation = Cont->GetBlackboard()->GetValueAsVector(BBKeys::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Cont, PlayerLocation);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
	
}