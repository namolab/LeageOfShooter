// Fill out your copyright notice in the Description page of Project Settings.


#include "StopInvestigate_BTTask.h"
#include "LeageOfShooter/Character/AI/Controller/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "LeageOfShooter/Character/AI/BlackboardKey.h"

UStopInvestigate_BTTask::UStopInvestigate_BTTask()
{
	NodeName = TEXT("Stop Investigate");
}

EBTNodeResult::Type UStopInvestigate_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const Cont = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (IsValid(Cont))
	{
		Cont->GetBlackboard()->SetValueAsBool(BBKeys::IsInvestigating,false);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}