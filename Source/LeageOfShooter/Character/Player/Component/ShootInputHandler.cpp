// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootInputHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"


UShootInputHandler::UShootInputHandler()
{
	
}

void UShootInputHandler::BeginPlay()
{
	Super::BeginPlay();
}

void UShootInputHandler::SetupInputHandler(ABaseCharacter* MyCharacter, UInputComponent* InputComponent)
{
	InputHandler = InputComponent;

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(MyCharacter))
	{
		OwnerCharacter = ShoPlayerCharacter;
	}

	InputHandler->BindAxis("MoveRight", this, &UShootInputHandler::MoveRight);
	InputHandler->BindAxis("MoveForward", this, &UShootInputHandler::MoveForward);
	InputHandler->BindAxis("Turn", this, &UShootInputHandler::Turn);
	InputHandler->BindAxis("TurnRate", this, &UShootInputHandler::TurnAtRate);
	InputHandler->BindAxis("LookUp", this, &UShootInputHandler::LookUp);
	InputHandler->BindAxis("LookUpRate", this, &UShootInputHandler::LookUpAtRate);

	InputHandler->BindAction("Jump", IE_Pressed, this, &UShootInputHandler::StartJump);
	InputHandler->BindAction("Jump", IE_Released, this, &UShootInputHandler::StopJump);

	InputHandler->BindAction("Crouch", IE_Pressed, this, &UShootInputHandler::StartCrouch);
	InputHandler->BindAction("Crouch", IE_Released, this, &UShootInputHandler::StopCrouch);

	InputHandler->BindAction("Sprint", IE_Pressed, this, &UShootInputHandler::StartSprint);
	InputHandler->BindAction("Sprint", IE_Released, this, &UShootInputHandler::StopSprint);

	InputHandler->BindAction("StartFire", IE_Pressed, this, &UShootInputHandler::FireButtonPressed);
	InputHandler->BindAction("StartFire", IE_Released, this, &UShootInputHandler::FireButtonReleased);

	InputHandler->BindAction("AimingButton", IE_Pressed, this, &UShootInputHandler::AimingButtonPressed);
	InputHandler->BindAction("AimingButton", IE_Released, this, &UShootInputHandler::AimingButtonReleased);

	InputHandler->BindAction("Interactive", IE_Pressed, this, &UShootInputHandler::InteractiveButtonPressed);

	InputHandler->BindAction("Reload", IE_Pressed, this, &UShootInputHandler::ReloadButtonPressed);

	InputHandler->BindAction("Drop", IE_Pressed, this, &UShootInputHandler::DropButtonPressed);

	InputHandler->BindAction("Tab", IE_Pressed, this, &UShootInputHandler::TabButtonPressed);
}

void UShootInputHandler::MoveForward(float Value)
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	if ((OwnerCharacter->Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = OwnerCharacter->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		OwnerCharacter->AddMovementInput(Direction, Value);
	}
}

void UShootInputHandler::MoveRight(float Value)
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	if ((OwnerCharacter->Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = OwnerCharacter->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		OwnerCharacter->AddMovementInput(Direction, Value);
	}
}

void UShootInputHandler::TurnAtRate(float Rate)
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->AddControllerYawInput(Rate * OwnerCharacter->BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void UShootInputHandler::LookUpAtRate(float Rate)
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->AddControllerPitchInput(Rate * OwnerCharacter->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void UShootInputHandler::Turn(float Rate)
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	float TurnScaleFactor = 0.0f;
	if (OwnerCharacter->GetIsAiming())
	{
		TurnScaleFactor = OwnerCharacter->MouseAimingTurnRate;
	}
	else
	{
		TurnScaleFactor = OwnerCharacter->MouseHipTurnRate;
	}
	OwnerCharacter->AddControllerYawInput(Rate * TurnScaleFactor);
}

void UShootInputHandler::LookUp(float Rate)
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	float LookUpScaleFactor = 0.0f;
	if (OwnerCharacter->GetIsAiming())
	{
		LookUpScaleFactor = OwnerCharacter->MouseAimingLookUpRate;
	}
	else
	{
		LookUpScaleFactor = OwnerCharacter->MouseHipLookUpRate;
	}
	OwnerCharacter->AddControllerPitchInput(Rate * LookUpScaleFactor);	
}

void UShootInputHandler::StartJump()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->JumpButtonPressed();	
}

void UShootInputHandler::StopJump()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->JumpButtonReleased();
}

void UShootInputHandler::StartCrouch()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->CrouchButtonPressed();	
}

void UShootInputHandler::StopCrouch()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->CrouchButtonReleased();
}

void UShootInputHandler::StartSprint()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->SprintButtonPressed();
}

void UShootInputHandler::StopSprint()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->SprintButtonReleased();
}

void UShootInputHandler::FireButtonPressed()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->FireButtonPressed();	
}

void UShootInputHandler::FireButtonReleased()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->FireButtonReleased();
}

void UShootInputHandler::AimingButtonPressed()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->AimingButtonPressed();
}

void UShootInputHandler::AimingButtonReleased()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->AimingButtonReleased();
}

void UShootInputHandler::InteractiveButtonPressed()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->InteractiveButtonPressed();
}

void UShootInputHandler::DropButtonPressed()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->DropButtonPressed();
}

void UShootInputHandler::ReloadButtonPressed()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->ReloadButtonPressed();
}

void UShootInputHandler::TabButtonPressed()
{
	if (!IsValid(OwnerCharacter) || OwnerCharacter->GetIsDie())
	{
		return;
	}

	OwnerCharacter->TabButtonPressed();
}