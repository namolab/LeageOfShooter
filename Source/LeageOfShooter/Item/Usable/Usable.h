// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeageOfShooter/Item/Item.h"
#include "LeageOfShooter/InteractableInterface.h"
#include "Usable.generated.h"


UCLASS()
class LEAGEOFSHOOTER_API AUsable : public AItem, public IInteractableInterface
{
	GENERATED_BODY()
	

public:
	AUsable();

	virtual void Interact() override;

protected:
	virtual void BeginPlay() override;
};
