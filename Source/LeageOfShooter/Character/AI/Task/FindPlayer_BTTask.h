// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPlayer_BTTask.generated.h"


UCLASS(Blueprintable)
class LEAGEOFSHOOTER_API UFindPlayer_BTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UFindPlayer_BTTask();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search")
	bool SearchRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search")
	float SearchRandius = 150.0f;
};
