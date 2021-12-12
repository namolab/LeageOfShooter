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
	OwnerCharacter = MyCharacter;

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
}

void UShootInputHandler::MoveForward(float Value)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if ((OwnerCharacter->Controller != nullptr) && (Value != 0.0f))
	{
		//find out which way is forward
		const FRotator Rotation = OwnerCharacter->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
		{
			if (ShoPlayerCharacter->MoveState == EMovementState::Walk)
			{
				Value *= 0.5f;
			}

			if (OwnerCharacter->GetCharacterMovement()->IsCrouching())
			{
				Value *= 0.4f;
			}

			if (ShoPlayerCharacter->GetIsAiming())
			{
				Value *= 0.8f;
			}
		}

		OwnerCharacter->AddMovementInput(Direction, Value);
	}
}

void UShootInputHandler::MoveRight(float Value)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if ((OwnerCharacter->Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = OwnerCharacter->Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
		{
			if (ShoPlayerCharacter->MoveState == EMovementState::Walk)
			{
				Value *= 0.5f;
			}

			if (OwnerCharacter->GetCharacterMovement()->IsCrouching())
			{
				Value *= 0.4f;
			}

			if (ShoPlayerCharacter->GetIsAiming())
			{
				Value *= 0.8f;
			}
		}

		// add movement in that direction
		OwnerCharacter->AddMovementInput(Direction, Value);
	}

}

void UShootInputHandler::TurnAtRate(float Rate)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		OwnerCharacter->AddControllerYawInput(Rate * ShoPlayerCharacter->BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void UShootInputHandler::LookUpAtRate(float Rate)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		OwnerCharacter->AddControllerPitchInput(Rate * ShoPlayerCharacter->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void UShootInputHandler::Turn(float Rate)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		float TurnScaleFactor = 0.0f;
		if (ShoPlayerCharacter->GetIsAiming())
		{
			TurnScaleFactor = ShoPlayerCharacter->MouseAimingTurnRate;
		}
		else
		{
			TurnScaleFactor = ShoPlayerCharacter->MouseHipTurnRate;
		}
		OwnerCharacter->AddControllerYawInput(Rate * TurnScaleFactor);
	}

}

void UShootInputHandler::LookUp(float Rate)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		float LookUpScaleFactor = 0.0f;
		if (ShoPlayerCharacter->GetIsAiming())
		{
			LookUpScaleFactor = ShoPlayerCharacter->MouseAimingLookUpRate;
		}
		else
		{
			LookUpScaleFactor = ShoPlayerCharacter->MouseHipLookUpRate;
		}
		OwnerCharacter->AddControllerPitchInput(Rate * LookUpScaleFactor);
	}
}

void UShootInputHandler::StartJump()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}
	OwnerCharacter->Jump();
}

void UShootInputHandler::StopJump()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}
	OwnerCharacter->StopJumping();
}

void UShootInputHandler::StartCrouch()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (!OwnerCharacter->GetCharacterMovement()->IsCrouching() && !OwnerCharacter->GetCharacterMovement()->IsFalling())
	{
		OwnerCharacter->GetCharacterMovement()->bWantsToCrouch = true;
	}
}

void UShootInputHandler::StopCrouch()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (OwnerCharacter->GetCharacterMovement()->IsCrouching())
	{
		OwnerCharacter->GetCharacterMovement()->bWantsToCrouch = false;
	}	
}

void UShootInputHandler::StartSprint()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		ShoPlayerCharacter->MoveState = EMovementState::Run;

	}
}

void UShootInputHandler::StopSprint()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		ShoPlayerCharacter->MoveState = EMovementState::Walk;

	}
}

void UShootInputHandler::FireButtonPressed()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		//ShoPlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Fire"))));
		ShoPlayerCharacter->FireButtonPressed();
	}

}

void UShootInputHandler::FireButtonReleased()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		ShoPlayerCharacter->FireButtonReleased();
	}
}

void UShootInputHandler::AimingButtonPressed()
{
	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		ShoPlayerCharacter->AimingButtonPressed();
	}
}

void UShootInputHandler::AimingButtonReleased()
{
	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		ShoPlayerCharacter->AimingButtonReleased();
	}
}
