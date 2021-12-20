// Fill out your copyright notice in the Description page of Project Settings.


#include "AggroEnemy_BTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


#include "LeageOfShooter/Character/AI/Controller/BaseAIController.h"
#include "LeageOfShooter/Character/AI/BlackboardKey.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"


UAggroEnemy_BTask::UAggroEnemy_BTask()
{
	NodeName = TEXT("AggroEnemy");
}

EBTNodeResult::Type UAggroEnemy_BTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cont = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (IsValid(Cont))
	{
		if (AAICharacter* const Enemy = Cast<AAICharacter>(Cont->GetPawn()))
		{
			Enemy->AggroEnemy();

			FTimerHandle AggroRateHandle;
			GetWorld()->GetTimerManager().SetTimer(AggroRateHandle,this, &UAggroEnemy_BTask::FinishAggro, 2.0f, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;

}

void UAggroEnemy_BTask::FinishAggro()
{
	Cont->GetBlackboard()->SetValueAsBool(BBKeys::AggroEnemy, false);
}