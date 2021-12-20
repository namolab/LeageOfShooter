// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "LeageOfShooter/Item/Weapon/RangeWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetWork.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	bIsInAir = false;
	bIsAccelerating = false;
	bCrouch = false;
	bAiming = false;
	bReloading = false;
	bTurnInPlace = false;
	bShouldUseFabrik = false;
	Speed = 0.0f;
	TIPCharacterYaw = 0.0f;
	MovementOffsetYaw = 0.0f;
	LastMovementOffsetYaw = 0.0f;
	TIPCharacterYawLastFrame = 0.0f;
	CharacterRotation = FRotator(0.0f);
	CharacterRotationLastFrame = FRotator(0.0f);
	RootYawOffset = 0.0f;
	RootPitch = 0.0f;
	YawDelta = 0.0f;
	RecoilWeight = 1.0f;
	EquipWeaponType = EWeaponType::SMG;
	OffsetState = EOffsetState::Hip;
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
	{
		return;
	}

	FVector Velocity = ShooterCharacter->GetVelocity();
	bReloading = (ShooterCharacter->GetCombatState() == EFireState::Reloading);
	bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();
	bAiming = ShooterCharacter->GetIsAiming();
	bCrouch = ShooterCharacter->GetIsCrouch();
	bShouldUseFabrik = (ShooterCharacter->GetCombatState() == EFireState::Idle) || 
		(ShooterCharacter->GetCombatState() == EFireState::FireInProgress);

	Velocity.Z = 0;
	Speed = Velocity.Size();

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

	if (bReloading)
	{
		OffsetState = EOffsetState::Reloading;
	}
	else if (bIsInAir)
	{
		OffsetState = EOffsetState::InAir;
	}
	else if (bAiming)
	{
		OffsetState = EOffsetState::Aiming;
	}
	else
	{
		OffsetState = EOffsetState::Hip;
	}

	if (IsValid(ShooterCharacter->GetEquippedWeapon()))
	{
		EquipWeaponType = ShooterCharacter->GetEquippedWeapon()->GetEWeaponType();
	}

	UpdateTurnInPlace();
	Lean(DeltaTime);
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

	if (Speed > 0 || bIsInAir)
	{
		RootYawOffset = 0.0f;
		TIPCharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		RotationCurveLastFrame = 0.0f;
		RotationCurve = 0.0f;
	}
	else
	{
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		TIPCharacterYaw = ShooterCharacter->GetActorRotation().Yaw;
		const float TIPYawDelta{ TIPCharacterYaw - TIPCharacterYawLastFrame };

		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);

		const float Turning = GetCurveValue(TEXT("Turning"));

		if (Turning > 0)
		{
			bTurnInPlace = true;
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
		else
		{
			bTurnInPlace = false;
		}
	}

	if (bTurnInPlace)
	{
		if (bReloading)
		{
			RecoilWeight = 1.0f;
		}
		else
		{
			RecoilWeight = 0.0f;
		}
	}
	else
	{
		if (bCrouch)
		{
			if (bReloading)
			{
				RecoilWeight = 1.0f;
			}
			else
			{
				RecoilWeight = 0.1f;
			}
		}
		else
		{
			if (bAiming || bReloading)
			{
				RecoilWeight = 1.0f;
			}
			else
			{
				RecoilWeight = 0.5f;
			}
		}
	}

	if (!ShooterCharacter->HasAuthority())
	{
		CS_SetSettingValue(RootYawOffset, RootPitch);
	}
}

void UPlayerAnimInstance::Lean(float DeltaTime)
{
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = ShooterCharacter->GetActorRotation();

	const FRotator Delta{ UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation,CharacterRotationLastFrame) };

	const float Target{ Delta .Yaw / DeltaTime };
	const float Interp{ FMath::FInterpTo(YawDelta,Target,DeltaTime,6.f) };
	YawDelta = FMath::Clamp(Interp, -90.0f, 90.0f);
}

void UPlayerAnimInstance::CS_SetSettingValue_Implementation(float RootYawOffsetParam, float RootPitchParam)
{
	RootYawOffset = RootYawOffsetParam;
	RootPitch = RootPitchParam;
}

