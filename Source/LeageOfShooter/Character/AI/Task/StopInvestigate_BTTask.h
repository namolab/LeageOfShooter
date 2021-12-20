// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "StopInvestigate_BTTask.generated.h"


UCLASS(Blueprintable)
class LEAGEOFSHOOTER_API UStopInvestigate_BTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UStopInvestigate_BTTask();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
