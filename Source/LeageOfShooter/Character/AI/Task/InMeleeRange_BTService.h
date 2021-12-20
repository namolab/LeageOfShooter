// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "InMeleeRange_BTService.generated.h"


UCLASS(Blueprintable)
class LEAGEOFSHOOTER_API UInMeleeRange_BTService : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UInMeleeRange_BTService();
	void OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	float MeleeRange;

	UPROPERTY(Transient)
	class ABaseAIController* Cont;
};
