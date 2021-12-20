// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAIController.h"
#include "KrakenAIController.generated.h"

/**
 * 
 */
UCLASS()
class LEAGEOFSHOOTER_API AKrakenAIController : public ABaseAIController
{
	GENERATED_BODY()
	

public:
	AKrakenAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
