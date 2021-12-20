// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "ChasePlayer_BTask.generated.h"


UCLASS(Blueprintable)
class LEAGEOFSHOOTER_API UChasePlayer_BTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UChasePlayer_BTask();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
