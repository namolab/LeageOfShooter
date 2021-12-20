// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomRocation_BTTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#include "LeageOfShooter/Character/AI/Controller/BaseAIController.h"
#include "LeageOfShooter/Character/AI/BlackboardKey.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"


UFindRandomRocation_BTTask::UFindRandomRocation_BTTask()
{
	NodeName = TEXT("Find Random Location");
	CheckFlag = false;
}

EBTNodeResult::Type UFindRandomRocation_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cont = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (IsValid(Cont))
	{
		if (AAICharacter* const Enemy = Cast<AAICharacter>(Cont->GetPawn()))
		{
			FVector const Origin = Enemy->GetActorLocation();
			FNavLocation Loc;

			UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

			if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Loc, nullptr))
			{
				Cont->GetBlackboard()->SetValueAsVector(BBKeys::TargetLocation, Loc.Location);
			}

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}