// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeageOfShooter/Character/BaseCharacter.h"
#include "LeageOfShooter/Item/AmmoType.h"
#include "PlayerCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoChanged, int32, MagazineAmmo, int32, TotalAmmo);


UENUM(BlueprintType)
enum class EFireState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	FireInProgress  UMETA(DisplayName = "FireInProgress"),
	Reloading  UMETA(DisplayName = "Reloading")
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

	void IncrementOverlappedItemCount(int32 Amount);

	void AimingButtonPressed();
	void AimingButtonReleased();
	void CrouchButtonPressed();
	void CrouchButtonReleased();
	void SprintButtonPressed();
	void SprintButtonReleased();
	void JumpButtonPressed();
	void JumpButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void InteractiveButtonPressed();
	void DropButtonPressed();
	void ReloadButtonPressed();

	UFUNCTION(BlueprintCallable)
	void TabButtonPressed();

	void FireWeapon();

	void ReloadWeapon();
	int32 CarryingAmmo();

	bool TraceUnderCrosshairs(FHitResult& OutHitResult,FVector& OutHitLocation, float LineDistance);

	void TraceForItems();

	void EquipWeapon(ARangeWeapon* WeaponToEquip);

	void DropItem();

	UFUNCTION(BlueprintCallable)
	void RefreshAmmoWidget();

	bool WeaponHasAmmo();
	void FinishReload();
	
	void GetPickupItem(class AItem* Item);
	void GetPickupAmmo(class AAmmo* Ammo);
	void GetPickupUsable(class AUsable* Usable);

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const { return CrosshairSpreadMultiplier; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UShootInputHandler* GetInputHandler() const { return InputHandlerComponent; }
	FORCEINLINE class USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE class UParticleSystem* GetFireParticle() const { return FireParticle; }
	FORCEINLINE class UAnimMontage* GetHipFireMontage() const { return HipFireMontage; }
	FORCEINLINE bool GetIsAiming() const { return bAiming; }
	FORCEINLINE bool GetIsCrouch() const { return bCrouch; }
	FORCEINLINE class ARangeWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
	FORCEINLINE EFireState GetCombatState() const { return CombatState; }
	FORCEINLINE bool GetIsDie() const { return bIsDie; }
	FORCEINLINE int32 GetOverlappedItemCount() const { return OverlappedItemCount; }

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnRep_AttachWeapon();

	UFUNCTION()
	void OnRep_Die();

	UFUNCTION(Server, Reliable)
	void CS_FireWeapon(const FVector& BeamEndParam, bool bBeamEndParam);
	UFUNCTION(NetMulticast, Reliable)
	void SM_FireWeapon(const FVector& BeamEndParam, bool bBeamEndParam);

	UFUNCTION(Server, Reliable)
	void CS_HitDamage(FHitResult HitResult, TSubclassOf<class UGameplayEffect> DamageEffect);
	UFUNCTION(NetMulticast, Reliable)
	void SM_HitDamage(FHitResult HitResult, TSubclassOf<class UGameplayEffect> DamageEffect);

	UFUNCTION(Server, Reliable)
	void CS_SetAiming(bool WantsToAim);

	UFUNCTION(Server, Reliable)
	void CS_SetCrouch(bool WantsToCrouch);

	UFUNCTION(Server, Reliable)
	void CS_EquipWeapon(ARangeWeapon* Weapon);

	UFUNCTION(Server, Reliable)
	void CS_DropItem();
	UFUNCTION(NetMulticast, Reliable)
	void SM_DropItem();

	UFUNCTION(Server, Reliable)
	void CS_ReloadWeapon();
	UFUNCTION(NetMulticast, Reliable)
	void SM_ReloadWeapon();

	UFUNCTION(Server, Reliable)
	void CS_GetPickupAmmo(AAmmo* Ammo);

	UFUNCTION(Server, Reliable)
	void CS_GetPickupUsable(class AUsable* Usable);

	UFUNCTION(Server, Reliable)
	void SM_GetPickupUsable(class AUsable* Usable);

	UFUNCTION()
	void FinishCrosshairBulletFire();

	UFUNCTION()
	void AutoFireReset();

	virtual void Die() override;

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);

	class ARangeWeapon* SpawnDefaultWeapon();

	void InitializeAmmoMap();

	void PlayFireSound();

	void SendBullet(const FVector& BeamEndParam, bool bBeamEndParam);

	void PlayGunFireMontage();

	void StartAim();
	void StopAim();

	void SwapWeapon(ARangeWeapon* Weapon);
	void StartCrosshairBulletFire();
	void StartFireTimer();


private:
	void UpdateCrouchCamLocation(float DeltaTime);
	void UpdateAiming(float DeltaTime);
	void UpdateLookRates(float DeltaTime);
	void UpdateCrosshairSpread(float DeltaTime);

	void HitDamage(FHitResult HitResult, TSubclassOf<class UGameplayEffect> DamageEffect);

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
	UPROPERTY(EditAnywhere, Category = Combat)
	TSoftObjectPtr<UAnimMontage> ReloadMontageSoft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	TSubclassOf<class UGameplayEffect> SprintConsumeEffect;

protected:
	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(Replicated)
	FVector BeamEnd;
	
	UPROPERTY(Replicated)
	bool bBeamEnd;

	UPROPERTY(Replicated)
	bool bCrouch;

	UPROPERTY(ReplicatedUsing = OnRep_AttachWeapon)
	class ARangeWeapon* EquippedWeapon;

	UPROPERTY(BlueprintAssignable)
	FAmmoChanged OnAmmoChanged;

	UPROPERTY(ReplicatedUsing = OnRep_Die)
	bool bIsDie;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InputComponent, meta = (AllowPrivateAccess = "true"))
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
	UPROPERTY(Transient)
	class UAnimMontage* ReloadMontage;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitLastFrame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARangeWeapon> DefaultWeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	int32 StartingAmmo9mm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	int32 StartingAmmoAR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EFireState CombatState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float BaseMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float SprintMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CrouchMovementSpeed;

	float ShootTimeDuration;
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	float CameraDefaultFOV;
	float CameraZoomedFOV;
	float CameraCurrentFOV;

	bool bShouldFire;
	float AutomaticFireRate;

	FTimerHandle AutoFireTimer;
	bool bShouldTraceForItems;
	int32 OverlappedItemCount;

	float bAimingPressed;

	bool bFireButtonPressed;
	FVector CamCurrentLocation;
	FVector CamBaseLocation;

	bool bToggleInventory;
};
