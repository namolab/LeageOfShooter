// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "FindRandomRocation_BTTask.generated.h"

UCLASS(Blueprintable)
class LEAGEOFSHOOTER_API UFindRandomRocation_BTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UFindRandomRocation_BTTask();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	FTimerHandle DecreaseGaugeHandle;

	UPROPERTY(Transient)
	class ABaseAIController* Cont;

	bool CheckFlag;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 1200.0f;
};
