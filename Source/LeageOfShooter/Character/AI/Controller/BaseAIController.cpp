// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

#include "LeageOfShooter/Character/AI/BlackboardKey.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "LeageOfShooter/Character/AI/Ai_Tags.h"

ABaseAIController::ABaseAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	SightRadValue = 600.0f;
	PeripheralVisionAngleDegree = 60.0f;
	HearingRange = 1500.0f;
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Btree))
	{
		RunBehaviorTree(Btree);
		BehaviorTreeComponent->StartTree(*Btree);
	}
}

void ABaseAIController::OnPossess(APawn* const ParamPawn)
{
	Super::OnPossess(ParamPawn);
	if (IsValid(Blackboard))
	{
		Blackboard->InitializeBlackboard(*Btree->BlackboardAsset);
	}
}

UBlackboardComponent* ABaseAIController::GetBlackboard() const
{
	return Blackboard;
}

void ABaseAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (IsValid(SightConfig))
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = SightRadValue;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegree;
		SightConfig->SetMaxAge(5.0f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = SightConfig->SightRadius + 200.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (IsValid(HearingConfig))
	{
		HearingConfig->HearingRange = HearingRange;
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnUpdated);
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}
}

void ABaseAIController::OnUpdated(TArray<AActor*> const& Updated_Actors)
{
	AAICharacter* GetOwnerAI = Cast<AAICharacter>(GetPawn());

	if (IsValid(GetOwnerAI))
	{
		if (GetOwnerAI->GetIsDie())
		{
			return;
		}
	}
	else
	{
		return;
	}

	for (int32 i = 0; i < Updated_Actors.Num(); i++)
	{
		FActorPerceptionBlueprintInfo Info;
		GetPerceptionComponent()->GetActorsPerception(Updated_Actors[i], Info);

		for (int32 j = 0; j < Info.LastSensedStimuli.Num(); j++)
		{
			FAIStimulus const Stim = Info.LastSensedStimuli[j];
			if (Stim.Tag == Tags::Noise_Tag)
			{
				GetBlackboard()->SetValueAsBool(BBKeys::IsInvestigating, Stim.WasSuccessfullySensed());
				GetBlackboard()->SetValueAsVector(BBKeys::TargetLocation, Stim.StimulusLocation);
			}
			else if (Stim.Type.Name == "Default__AISense_Sight")
			{
				if (auto const DetecActor = Cast<APlayerCharacter>(Updated_Actors[i]))
				{
					UObject* TargetChar = GetBlackboard()->GetValueAsObject(BBKeys::TargetPlayer);
					bool bChekSeePlayer = GetBlackboard()->GetValueAsBool(BBKeys::CanSeePlayer);

					if (IsValid(TargetChar))
					{
						if (TargetChar->GetUniqueID() == DetecActor->GetUniqueID())
						{
							if (DetecActor->GetIsDie())
							{
								GetBlackboard()->SetValueAsObject(BBKeys::TargetPlayer, nullptr);
								GetBlackboard()->SetValueAsBool(BBKeys::CanSeePlayer, false);
								GetOwnerAI->VisibleHealthBar(false);	
							}
							else
							{
								if (!Stim.WasSuccessfullySensed())
								{
									GetBlackboard()->SetValueAsObject(BBKeys::TargetPlayer, nullptr);
								}

								GetBlackboard()->SetValueAsBool(BBKeys::CanSeePlayer, Stim.WasSuccessfullySensed());
								GetOwnerAI->VisibleHealthBar(Stim.WasSuccessfullySensed());			
							}
						}
					}
					else
					{
						GetBlackboard()->SetValueAsBool(BBKeys::CanSeePlayer, Stim.WasSuccessfullySensed());
						GetOwnerAI->VisibleHealthBar(Stim.WasSuccessfullySensed());

						if (!bChekSeePlayer)
						{
							if (GetBlackboard()->GetValueAsBool(BBKeys::CanSeePlayer))
							{
								GetBlackboard()->SetValueAsBool(BBKeys::AggroEnemy, true);
							}
						}		
					}
				}
			}
		}
	}
}