// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AILightAttack_BTTask.generated.h"


UCLASS(Blueprintable)
class LEAGEOFSHOOTER_API UAILightAttack_BTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UAILightAttack_BTTask();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool FinishAttack(class AAICharacter* AI);

private:
	UPROPERTY(Transient)
	class ABaseAIController* Cont;
};
