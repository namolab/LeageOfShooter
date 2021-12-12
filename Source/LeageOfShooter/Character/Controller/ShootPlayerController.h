// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseController.h"
#include "ShootPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEAGEOFSHOOTER_API AShootPlayerController : public ABaseController
{
	GENERATED_BODY()
	
public:
	AShootPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateListener();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> MainWidgetClass;

private:
	UPROPERTY(Transient)
	class UUserWidget* MainWidget;
};
