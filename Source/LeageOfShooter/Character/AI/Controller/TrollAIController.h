// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "TrollAIController.generated.h"

/**
 * 
 */
UCLASS()
class LEAGEOFSHOOTER_API ATrollAIController : public ABaseAIController
{
	GENERATED_BODY()
	
public:
	ATrollAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
