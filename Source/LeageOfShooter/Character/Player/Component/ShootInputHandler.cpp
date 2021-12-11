// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootInputHandler.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"

// Sets default values for this component's properties
UShootInputHandler::UShootInputHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

// Called when the game starts
void UShootInputHandler::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
//void UShootInputHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

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

	InputHandler->BindAction("StartFire", IE_Pressed, this, &UShootInputHandler::FireWeapon);

	//InputComponentRef->BindAction("ChangeToPistol", EInputEvent::IE_Pressed, PlayerCharacterRef.Get(), &APlayerCharacter::ChangeToPistol);
	//InputComponentRef->BindAction("ChangeToRifle", EInputEvent::IE_Pressed, PlayerCharacterRef.Get(), &APlayerCharacter::ChangeToRifle);
	//InputComponentRef->BindAction("ChangeToNone", EInputEvent::IE_Pressed, PlayerCharacterRef.Get(), &APlayerCharacter::ChangeToNone);
	//InputComponentRef->BindAction("IronSights", EInputEvent::IE_Pressed, PlayerCharacterRef.Get(), &APlayerCharacter::ONIronSights);
	//InputComponentRef->BindAction("IronSights", EInputEvent::IE_Released, PlayerCharacterRef.Get(), &APlayerCharacter::OFFIronSights);
	//InputComponentRef->BindAction("Shoot", EInputEvent::IE_Pressed, PlayerCharacterRef.Get(), &APlayerCharacter::Shoot);
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
			else if (ShoPlayerCharacter->MoveState == EMovementState::Crouch || OwnerCharacter->GetCharacterMovement()->IsCrouching())
			{
				Value *= 0.3f;
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
			else if (ShoPlayerCharacter->MoveState == EMovementState::Crouch || OwnerCharacter->GetCharacterMovement()->IsCrouching())
			{
				Value *= 0.3f;
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
	// calculate delta for this frame from the rate information
	OwnerCharacter->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void UShootInputHandler::LookUpAtRate(float Rate)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}
	// calculate delta for this frame from the rate information
	OwnerCharacter->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void UShootInputHandler::Turn(float Rate)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}
	OwnerCharacter->AddControllerYawInput(Rate);
}

void UShootInputHandler::LookUp(float Rate)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}
	OwnerCharacter->AddControllerPitchInput(Rate);
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
		if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
		{
			ShoPlayerCharacter->MoveState = EMovementState::Crouch;
		}

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
		if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
		{
			ShoPlayerCharacter->MoveState = EMovementState::Walk;
		}

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

void UShootInputHandler::FireWeapon()
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	if (APlayerCharacter* ShoPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		ShoPlayerCharacter->FireWeapon();
	}
}
