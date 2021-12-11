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
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

APlayerCharacter::APlayerCharacter()
	: Super()
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
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.0f, 50.0f);


	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InputHandlerComponent = CreateDefaultSubobject<UShootInputHandler>(TEXT("InputHandlerComponent"));

	MoveState = EMovementState::Walk;
	CombatState = ECombatState::Normal;
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

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCrouchCamLocation(DeltaTime);

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
}

void APlayerCharacter::FireWeapon()
{	
	if (HasAuthority())
	{
		SM_FireWeapon();
	}
	else
	{
		CS_FireWeapon();
	}
}


void APlayerCharacter::CS_FireWeapon_Implementation()
{
	FireWeapon();
}

void APlayerCharacter::SM_FireWeapon_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	
	if (IsValid(GetMesh()))
	{
		const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");

		if (BarrelSocket)
		{
			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, SocketTransform);

			FVector2D ViewPortSize;
			if (GEngine && GEngine->GameViewport)
			{
				GEngine->GameViewport->GetViewportSize(ViewPortSize);
			}

			FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
			CrosshairLocation.Y -= 45.f;
			CrosshairLocation.X -= 25.f;

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

				FVector BeamEndPoint = End;

				GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
			
				if (ScreenTraceHit.bBlockingHit)
				{
					BeamEndPoint = ScreenTraceHit.Location;

					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, ScreenTraceHit.Location);

				}

				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireBeamParticle, SocketTransform);

				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEndPoint);
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