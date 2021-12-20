// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayer_BTTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"

#include "LeageOfShooter/Character/AI/Controller/BaseAIController.h"
#include "LeageOfShooter/Character/AI/BlackboardKey.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"

UFindPlayer_BTTask::UFindPlayer_BTTask()
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayer_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const Cont = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(Cont))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	if (AAICharacter* const Enemy = Cast<AAICharacter>(Cont->GetPawn()))
	{
		TArray<AActor*> TheActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), TheActors);

		float MinDistance = FLT_MAX;
		FVector PlayerLocation;
		APlayerCharacter* TargetPlayer = nullptr;

		for (auto Actor : TheActors)
		{
			if (auto const PlayerChar = Cast<APlayerCharacter>(Actor))
			{
				if (PlayerChar->GetIsDie())
				{
					continue;
				}

				float DistanceFromEnemy = Enemy->GetDistanceTo(PlayerChar);

				if (MinDistance > DistanceFromEnemy)
				{
					MinDistance = DistanceFromEnemy;
					TargetPlayer = PlayerChar;
				}
			}
		}

		if (IsValid(TargetPlayer))
		{
			FNavLocation NavLoc;
			UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			FVector const TargetLoc = TargetPlayer->GetActorLocation();

			if (NavSys->GetRandomPointInNavigableRadius(TargetLoc, SearchRandius, NavLoc, nullptr))
			{
				Cont->GetBlackboard()->SetValueAsVector(BBKeys::TargetLocation, NavLoc.Location);
			}
				
			Cont->GetBlackboard()->SetValueAsObject(BBKeys::TargetPlayer, TargetPlayer);			

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}	

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;	
}

