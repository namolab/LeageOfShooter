// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetWork.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	Speed = 0.0f;
	bIsInAir = false;
	bIsAccelerating = false;
	MovementOffsetYaw = 0.0f;
	LastMovementOffsetYaw = 0.0f;
	bAiming = false;
	CharacterYaw = 0.0f;
	CharacterYawLastFrame = 0.0f;
	RootYawOffset = 0.0f;
	RootPitch = 0.0f;
}


void UPlayerAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerAnimInstance, RootYawOffset);
	DOREPLIFETIME(UPlayerAnimInstance, RootPitch);
}

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

	FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());

	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	if (ShooterCharacter->GetVelocity().Size() > 0.f)
	{
		LastMovementOffsetYaw = MovementOffsetYaw;
	}

	bAiming = ShooterCharacter->GetIsAiming();

	UpdateTurnInPlace();
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::UpdateTurnInPlace()
{
	RootPitch = ShooterCharacter->GetBaseAimRotation().Pitch;
	
	if (RootPitch > 180.0f)
	{
		RootPitch = RootPitch - 360.0f;
	}

	if (Speed > 0)
	{
		RootYawOffset = 0.0f;
		CharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		CharacterYawLastFrame = CharacterYaw;
		RotationCurveLastFrame = 0.0f;
		RotationCurve = 0.0f;
	}
	else
	{
		CharacterYawLastFrame = CharacterYaw;
		CharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		const float YawDelta{ CharacterYaw - CharacterYawLastFrame };

		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - YawDelta);

		const float Turning = GetCurveValue(TEXT("Turning"));

		if (Turning > 0)
		{
			RotationCurveLastFrame = RotationCurve;
			RotationCurve = GetCurveValue(TEXT("Rotation"));
			const float DeltaRotaion = RotationCurve - RotationCurveLastFrame;

			if (RootYawOffset > 0)
			{
				RootYawOffset -= DeltaRotaion;
			}
			else
			{
				RootYawOffset += DeltaRotaion;
			}

			const float ABSRootYawOffset = FMath::Abs(RootYawOffset);

			if (ABSRootYawOffset > 90.0f)
			{
				const float YawExcess = ABSRootYawOffset - 90.0f;

				if (RootYawOffset > 0)
				{
					RootYawOffset -= YawExcess;
				}
				else
				{
					RootYawOffset += YawExcess;
				}

			}
		}
	}

	if (!ShooterCharacter->HasAuthority())
	{
		CS_SetSettingValue(RootYawOffset, RootPitch);
	}
}

void UPlayerAnimInstance::CS_SetSettingValue_Implementation(float RootYawOffsetParam, float RootPitchParam)
{
	RootYawOffset = RootYawOffsetParam;
	RootPitch = RootPitchParam;
}

