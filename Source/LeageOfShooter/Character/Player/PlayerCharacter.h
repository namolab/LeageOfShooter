// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeageOfShooter/Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Run  UMETA(DisplayName = "Run"),
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Battle  UMETA(DisplayName = "Battle")
};


UCLASS()
class LEAGEOFSHOOTER_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UShootInputHandler* GetInputHandler() const { return InputHandlerComponent; }

	FORCEINLINE class USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE class UParticleSystem* GetFireParticle() const { return FireParticle; }
	FORCEINLINE class UAnimMontage* GetHipFireMontage() const { return HipFireMontage; }
	FORCEINLINE bool GetIsAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const { return CrosshairSpreadMultiplier; }

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void CS_FireWeapon(const FVector& BeamEndParam, bool bBeamEndParam);

	UFUNCTION(NetMulticast, Reliable)
	void SM_FireWeapon(const FVector& BeamEndParam, bool bBeamEndParam);

	UFUNCTION(Server, Reliable)
	void CS_HitDamage(FHitResult HitResult);

	UFUNCTION(NetMulticast, Reliable)
	void SM_HitDamage(FHitResult HitResult);

	UFUNCTION(Server, Reliable)
	void CS_SetAiming(bool WantsToAim);

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

public:
	void AimingButtonPressed();
	void AimingButtonReleased();
	void FireWeapon();

	void StartCrosshairBulletFire();

	UFUNCTION()
	void FinishCrosshairBulletFire();

	void FireButtonPressed();
	void FireButtonReleased();

	void StartFireTimer();

	UFUNCTION()
	void AutoFireReset();


private:
	void UpdateCrouchCamLocation(float DeltaTime);
	void UpdateAiming(float DeltaTime);
	void UpdateLookRates(float DeltaTime);
	void UpdateCrosshairSpread(float DeltaTime);

	void HitDamage(FHitResult HitResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float HipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float HipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float AimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float AimingLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseHipLookUpRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimingLookUpRate;

	UPROPERTY(EditAnywhere)
	EMovementState MoveState;

	UPROPERTY(EditAnywhere)
	ECombatState CombatState;

	FVector CamCurrentLocation;
	FVector CamBaseLocation;

	UPROPERTY(EditAnywhere, Category= Combat)
	TSoftObjectPtr<USoundCue> FireSoundSoft;
	UPROPERTY(EditAnywhere, Category = Combat)
	TSoftObjectPtr<UParticleSystem> FireParticleSoft;
	UPROPERTY(EditAnywhere, Category = Combat)
	TSoftObjectPtr<UParticleSystem> ImpactParticleSoft;
	UPROPERTY(EditAnywhere, Category = Combat)
	TSoftObjectPtr<UParticleSystem> FireBeamParticleSoft;
	UPROPERTY(EditAnywhere, Category = Combat)
	TSoftObjectPtr<UAnimMontage> HipFireMontageSoft;

	bool bFireButtonPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	TSubclassOf<class UGameplayEffect> FireDamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	TSubclassOf<class UGameplayEffect> SprintConsumeEffect;

protected:
	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(Replicated)
	FVector BeamEnd;
	
	UPROPERTY(Replicated)
	bool bBeamEnd;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = InputComponent, meta = (AllowPrivateAccess = "true"))
	class UShootInputHandler* InputHandlerComponent;

	UPROPERTY(Transient)
	class USoundCue* FireSound;
	UPROPERTY(Transient)
	class UParticleSystem* FireParticle;
	UPROPERTY(Transient)
	class UParticleSystem* ImpactParticle;
	UPROPERTY(Transient)
	class UParticleSystem* FireBeamParticle;
	UPROPERTY(Transient)
	class UAnimMontage* HipFireMontage;

	float CameraDefaultFOV;
	float CameraZoomedFOV;
	float CameraCurrentFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor;

	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	bool bShouldFire;
	float AutomaticFireRate;


	FTimerHandle AutoFireTimer;
};
