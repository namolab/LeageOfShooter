// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class LEAGEOFSHOOTER_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void BeginPlay() override;
	void OnPossess(APawn* const ParamPawn) override;

	class UBlackboardComponent* GetBlackboard() const;

protected:
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& Updated_Actors);

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* Btree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SightRadValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PeripheralVisionAngleDegree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float HearingRange;

	class UBlackboardComponent* Blackboard;

	UPROPERTY(Transient)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(Transient)
	class UAISenseConfig_Hearing* HearingConfig;

};
