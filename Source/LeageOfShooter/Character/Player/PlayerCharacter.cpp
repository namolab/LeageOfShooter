// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LeageOfShooter/Character/Player/Component/ShootInputHandler.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetWork.h"
 
APlayerCharacter::APlayerCharacter()
	: Super(), CameraDefaultFOV(0.0f), CameraCurrentFOV(0.0f)
{
	//// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 180.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.0f, 70.0f);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InputHandlerComponent = CreateDefaultSubobject<UShootInputHandler>(TEXT("InputHandlerComponent"));

	MoveState = EMovementState::Walk;
	CombatState = ECombatState::Normal;
	CameraZoomedFOV = 35.0f;
	ZoomInterpSpeed = 20.0f;
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	HipTurnRate = 90.0f;
	HipLookUpRate = 90.0f;
	AimingTurnRate = 20.0f;
	AimingLookUpRate = 20.0f;

	MouseHipTurnRate = 1.0f;
	MouseHipLookUpRate = 1.0f;
	MouseAimingTurnRate = 0.8f;
	MouseAimingLookUpRate = 0.8f;


	CrosshairSpreadMultiplier = 0.0f;
	CrosshairVelocityFactor = 0.0f;
	CrosshairInAirFactor = 0.0f;
	CrosshairAimFactor = 0.0f;
	CrosshairShootingFactor = 0.0f;

	ShootTimeDuration = 0.05f;
	bFiringBullet = false;
	AutomaticFireRate = 0.1f;
	bShouldFire = true;
	bFireButtonPressed = false;
	bAiming = false;
	bBeamEnd = false;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, bAiming);
	DOREPLIFETIME(APlayerCharacter, BeamEnd);
	DOREPLIFETIME(APlayerCharacter, bBeamEnd);
}

void APlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	if (!GetCharacterMovement()->IsFalling())
	{
		const FVector ModifyVec(0, 0, ScaledHalfHeightAdjust);

		CamCurrentLocation = CamBaseLocation + ModifyVec;
	}
}

void APlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	if (!GetCharacterMovement()->IsFalling())
	{
		const FVector ModifyVec(0, 0, ScaledHalfHeightAdjust);

		CamCurrentLocation = CamBaseLocation - ModifyVec;
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCrouchCamLocation(DeltaTime);
	UpdateAiming(DeltaTime);
	UpdateLookRates(DeltaTime);
	UpdateCrosshairSpread(DeltaTime);
}



void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	if (IsValid(InputHandlerComponent))
	{
		InputHandlerComponent->SetupInputHandler(this, PlayerInputComponent);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CamBaseLocation = FollowCamera->GetRelativeLocation();
	CamCurrentLocation = CamBaseLocation;
	FireSound = FireSoundSoft.LoadSynchronous();
	FireParticle = FireParticleSoft.LoadSynchronous();
	HipFireMontage = HipFireMontageSoft.LoadSynchronous();
	ImpactParticle = ImpactParticleSoft.LoadSynchronous();
	FireBeamParticle = FireBeamParticleSoft.LoadSynchronous();

	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
}

bool APlayerCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);

	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		const FVector Start = CrosshairWorldPosition;
		const FVector End = CrosshairWorldPosition + CrosshairWorldDirection * 10000.f;

		OutBeamLocation = End;
		
		FCollisionQueryParams CollisionParams;
		FCollisionResponseParams CollisionResponese;
		CollisionParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_EngineTraceChannel2, CollisionParams, CollisionResponese);

		if (ScreenTraceHit.bBlockingHit)
		{
			OutBeamLocation = ScreenTraceHit.Location;
		}

		FHitResult WeaponTraceHit;
		const FVector WeaponTraceStart = MuzzleSocketLocation;
		const FVector WeaponTraceEnd = OutBeamLocation;
		

		GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_EngineTraceChannel2, CollisionParams, CollisionResponese);

		if (WeaponTraceHit.bBlockingHit)
		{
			OutBeamLocation = WeaponTraceHit.Location;
			HitDamage(WeaponTraceHit);
		}
		else if (ScreenTraceHit.bBlockingHit)
		{
			HitDamage(ScreenTraceHit);
		}

		return true;
	}

	return false;
}

void APlayerCharacter::AimingButtonPressed()
{
	bAiming = true;

	if (!HasAuthority())
	{
		CS_SetAiming(bAiming);
	}
}

void APlayerCharacter::AimingButtonReleased()
{
	bAiming = false;

	if (!HasAuthority())
	{
		CS_SetAiming(bAiming);
	}
}

void APlayerCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;

	GetWorldTimerManager().SetTimer(CrosshairShootTimer, this, &APlayerCharacter::FinishCrosshairBulletFire, ShootTimeDuration, false);
}

void APlayerCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

void APlayerCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	StartFireTimer();
}

void APlayerCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void APlayerCharacter::StartFireTimer()
{
	if (bShouldFire)
	{
		FireWeapon();
		bShouldFire = false;
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &APlayerCharacter::AutoFireReset, AutomaticFireRate, false);
	}
}

void APlayerCharacter::AutoFireReset()
{
	bShouldFire = true;
	if (bFireButtonPressed)
	{
		StartFireTimer();
	}
}

void APlayerCharacter::FireWeapon()
{
	bBeamEnd = false;

	if (IsValid(GetMesh()))
	{
		const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");

		if (BarrelSocket)
		{
			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
			bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);
		}
	}

	if (HasAuthority())
	{
		SM_FireWeapon(BeamEnd, bBeamEnd);
	}
	else
	{
		CS_FireWeapon(BeamEnd, bBeamEnd);
	}
}

void APlayerCharacter::CS_FireWeapon_Implementation(const FVector& BeamEndParam, bool bBeamEndParam)
{
	BeamEnd = BeamEndParam;
	bBeamEnd = bBeamEndParam;

	SM_FireWeapon(BeamEnd, bBeamEnd);
}

void APlayerCharacter::SM_FireWeapon_Implementation(const FVector& BeamEndParam, bool bBeamEndParam)
{
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	
	if (IsValid(GetMesh()))
	{
		const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");

		if (BarrelSocket)
		{
			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, SocketTransform);

			if (bBeamEndParam)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, BeamEndParam);
			
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireBeamParticle, SocketTransform);

				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEndParam);
				}
			}
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

	StartCrosshairBulletFire();
}

void APlayerCharacter::HitDamage(FHitResult HitResult)
{
	if (HasAuthority())
	{
		SM_HitDamage(HitResult);
	}
	else
	{
		CS_HitDamage(HitResult);
	}
}

void APlayerCharacter::CS_HitDamage_Implementation(FHitResult HitResult)
{
	SM_HitDamage(HitResult);
}

void APlayerCharacter::SM_HitDamage_Implementation(FHitResult HitResult)
{
	if (AActor* HitActor = HitResult.GetActor())
	{
		///////////////////////////////현재는 플레이어 끼리 데미지 입히는 구조////////////////////
		if (APlayerCharacter* HitActorPlayer = Cast<APlayerCharacter>(HitActor))
		{
			if (IsValid(FireDamageEffect))
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(FireDamageEffect, 1, EffectContext);

				AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), HitActorPlayer->GetAbilitySystemComponent());
			}
		}
	}
}

void APlayerCharacter::CS_SetAiming_Implementation(bool WantsToAim)
{
	bAiming = WantsToAim;
}

void APlayerCharacter::UpdateCrouchCamLocation(float DeltaTime)
{
	if (CamCurrentLocation == CamBaseLocation)
		return;

	FVectorSpringState SpringState;
	FVector SmoothVec = UKismetMathLibrary::VectorSpringInterp(CamCurrentLocation, CamBaseLocation, SpringState, 240.0f, 0.8f, DeltaTime, 6.2f);
	CamCurrentLocation = SmoothVec;

	FollowCamera->SetRelativeLocation(CamCurrentLocation);
}


void APlayerCharacter::UpdateAiming(float DeltaTime)
{
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraZoomedFOV, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, DeltaTime, ZoomInterpSpeed);
	}

	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
}

void APlayerCharacter::UpdateLookRates(float DeltaTime)
{
	if (bAiming)
	{
		BaseTurnRate = AimingTurnRate;
		BaseLookUpRate = AimingLookUpRate;
	}
	else
	{
		BaseTurnRate = HipTurnRate;
		BaseLookUpRate = HipLookUpRate;
	}
}

void APlayerCharacter::UpdateCrosshairSpread(float DeltaTime)
{
	FVector2D WalkSpeedRange = { 0.0f, 600.0f };
	FVector2D VelocityMultiplierRange{ 0.0f,1.0f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.f;

	CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(
		WalkSpeedRange,
		VelocityMultiplierRange,
		Velocity.Size()
	);

	if (GetCharacterMovement()->IsFalling())
	{
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 1.25f, DeltaTime, 2.25f);
	}
	else
	{
		CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.0f, DeltaTime, 30.0f);
	}

	if (bAiming)
	{
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.6f, DeltaTime, 30.0f);
	}
	else
	{
		CrosshairAimFactor = FMath::FInterpTo(CrosshairAimFactor, 0.0f, DeltaTime, 30.0f);
	}

	if (bFiringBullet)
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.3f, DeltaTime, 60.0f);
	}
	else
	{
		CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.0f, DeltaTime, 60.0f);
	}


	CrosshairSpreadMultiplier = 0.5f + 
		CrosshairVelocityFactor + CrosshairInAirFactor - 
		CrosshairAimFactor + CrosshairShootingFactor;
}
