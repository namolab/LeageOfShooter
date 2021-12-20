// Fill out your copyright notice in the Description page of Project Settings.


#include "AILightAttack_BTTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/KismetMathLibrary.h"

#include "LeageOfShooter/Character/AI/Controller/BaseAIController.h"
#include "LeageOfShooter/Character/AI/BlackboardKey.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"

UAILightAttack_BTTask::UAILightAttack_BTTask()
{
	NodeName = TEXT("LightAttackEnemy");
}

EBTNodeResult::Type UAILightAttack_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Cont = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (IsValid(Cont))
	{
		if (AAICharacter* const Enemy = Cast<AAICharacter>(Cont->GetPawn()))
		{
			if (UObject* TargetChar = Cont->GetBlackboard()->GetValueAsObject(BBKeys::TargetPlayer))
			{
				if (APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(TargetChar))
				{
					FRotator EnemyToPlayerRot =UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), TargetPlayer->GetActorLocation());
					FRotator EnemyRotator = Enemy->GetActorRotation();
					EnemyRotator.Yaw = EnemyToPlayerRot.Yaw;
					Enemy->SetActorRotation(EnemyRotator);
				}
			}

			if (FinishAttack(Enemy))
			{
				Enemy->LightAttackEnemy();
			}

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}			

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	return EBTNodeResult::Failed;
}

bool UAILightAttack_BTTask::FinishAttack(AAICharacter* AI)
{
	return AI->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(AI->GetLightAttackMontage());
}
