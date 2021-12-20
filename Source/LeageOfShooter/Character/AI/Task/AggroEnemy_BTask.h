// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AggroEnemy_BTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LEAGEOFSHOOTER_API UAggroEnemy_BTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UAggroEnemy_BTask();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void FinishAggro();

private:
	UPROPERTY(Transient)
	class ABaseAIController* Cont;
};
