// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootInputHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAGEOFSHOOTER_API UShootInputHandler : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShootInputHandler();

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void Turn(float Rate);
	void LookUp(float Rate);

	void StartJump();
	void StopJump();

	void StartCrouch();
	void StopCrouch();

	void StartSprint();
	void StopSprint();

	void FireButtonPressed();
	void FireButtonReleased();

	void AimingButtonPressed();
	void AimingButtonReleased();

	void InteractiveButtonPressed();

	void DropButtonPressed();

	void ReloadButtonPressed();
	void TabButtonPressed();
public:	
	void SetupInputHandler(class ABaseCharacter* MyCharacter, class UInputComponent* InputComponent);

private:
	UPROPERTY(Transient)
	UInputComponent* InputHandler;

	UPROPERTY(Transient)
	class APlayerCharacter* OwnerCharacter;
};
