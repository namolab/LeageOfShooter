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
	Crouch  UMETA(DisplayName = "Crouch")
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
	// Sets default values for this character's properties
	APlayerCharacter();

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UShootInputHandler* GetInputHandler() const { return InputHandlerComponent; }

	FORCEINLINE class USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE class UParticleSystem* GetFireParticle() const { return FireParticle; }
	FORCEINLINE class UAnimMontage* GetHipFireMontage() const { return HipFireMontage; }


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(Server, Reliable)
	void CS_FireWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void SM_FireWeapon();

public:
	void FireWeapon();

private:
	void UpdateCrouchCamLocation(float DeltaTime);

public:
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

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
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
};
