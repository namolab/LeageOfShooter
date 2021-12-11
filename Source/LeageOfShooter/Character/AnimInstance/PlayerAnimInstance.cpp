// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (!IsValid(ShooterCharacter))
	{
		ShooterCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}

	if (!IsValid(ShooterCharacter))
		return;

	FVector Velocity = ShooterCharacter->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

	if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
	{
		bIsAccelerating = true;
	}
	else
	{
		bIsAccelerating = false;
	}

}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

}
