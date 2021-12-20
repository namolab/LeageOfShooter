// Fill out your copyright notice in the Description page of Project Settings.


#include "InMeleeRange_BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


#include "LeageOfShooter/Character/AI/Controller/BaseAIController.h"
#include "LeageOfShooter/Character/AI/BlackboardKey.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"

UInMeleeRange_BTService::UInMeleeRange_BTService()
{
	MeleeRange = 90.0f;
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("InMeleeRangeCheck");
}

void UInMeleeRange_BTService::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);

	Cont = Cast<ABaseAIController>(owner_comp.GetAIOwner());

	if (IsValid(Cont))
	{
		if (AAICharacter* const Enemy = Cast<AAICharacter>(Cont->GetPawn()))
		{
			UObject* TargetChar = Cont->GetBlackboard()->GetValueAsObject(BBKeys::TargetPlayer);
			if (IsValid(TargetChar))
			{
				APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(TargetChar);

				if (IsValid(TargetPlayer))
				{
					float DistanceFromEnemy = Enemy->GetDistanceTo(TargetPlayer);
					Cont->GetBlackboard()->SetValueAsBool(BBKeys::PlayerInMeleeRange, (DistanceFromEnemy <= MeleeRange));
				}
			}
		}
	}
}