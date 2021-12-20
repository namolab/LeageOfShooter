// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LeageOfShooter/Item/WeaponType.h"
#include "PlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EOffsetState : uint8
{
	Aiming UMETA(DisplayName = "Aiming"),
	Hip  UMETA(DisplayName = "Hip"),
	Reloading  UMETA(DisplayName = "Reloading"),
	InAir  UMETA(DisplayName = "InAir")
};

UCLASS()
class LEAGEOFSHOOTER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintPure)
	float GetYawOffset() const { return RootYawOffset; }

	UFUNCTION(BlueprintPure)
	float GetRootPitch() const { return RootPitch; }

protected:
	void UpdateTurnInPlace();

	void Lean(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void CS_SetSettingValue(float RootYawOffsetParam, float RootPitchParam);

protected:
	UPROPERTY(Replicated)
	float RootYawOffset;

	UPROPERTY(Replicated)
	float RootPitch;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class APlayerCharacter* ShooterCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	float TIPCharacterYaw;
	float TIPCharacterYawLastFrame;

	float RotationCurve;
	float RotationCurveLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	EOffsetState OffsetState;

	FRotator CharacterRotation;
	FRotator CharacterRotationLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lean, meta = (AllowPrivateAccess = "true"))
	float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouch, meta = (AllowPrivateAccess = "true"))
	bool bCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouch, meta = (AllowPrivateAccess = "true"))
	float RecoilWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouch, meta = (AllowPrivateAccess = "true"))
	float bTurnInPlace;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouch, meta = (AllowPrivateAccess = "true"))
	EWeaponType EquipWeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Crouch, meta = (AllowPrivateAccess = "true"))
	bool bShouldUseFabrik;
};
