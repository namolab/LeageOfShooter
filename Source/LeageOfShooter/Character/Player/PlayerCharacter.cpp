// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LeageOfShooter/Character/Player/Component/ShootInputHandler.h"
#include "LeageOfShooter/Character/Player/Component/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "Perception/AISense_Hearing.h"
#include "LeageOfShooter/Character/AI/Ai_Tags.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"
#include "LeageOfShooter/Character/Player/Component/ShootInputHandler.h"
#include "LeageOfShooter/Character/Player/Controller/ShootPlayerController.h"
#include "LeageOfShooter/Item/Weapon/RangeWeapon.h"
#include "LeageOfShooter/Item/Ammo/Ammo.h"
#include "LeageOfShooter/Interactive/InteractiveActor.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetWork.h"
 
APlayerCharacter::APlayerCharacter()
	: Super(), CameraDefaultFOV(0.0f), CameraCurrentFOV(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false; 	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 240.0f; 
	CameraBoom->bUsePawnControlRotation = true; 
	CameraBoom->SocketOffset = FVector(0.f, 35.0f, 80.0f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false; 

	InputHandlerComponent = CreateDefaultSubobject<UShootInputHandler>(TEXT("InputHandlerComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	CameraZoomedFOV = 25.0f;
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
	
	StartingAmmo9mm = 85;
	StartingAmmoAR = 120;
	AutomaticFireRate = 0.1f;
	SprintMovementSpeed = 500.0f;
	BaseMovementSpeed = 350.0f;
	CrouchMovementSpeed = 200.0f;

	CombatState = EFireState::Idle;
	bFiringBullet = false;

	bShouldFire = true;
	bFireButtonPressed = false;
	bAiming = false;
	bBeamEnd = false;
	bShouldTraceForItems = false;
	bCrouch = false;
	bAimingPressed = false;
	bToggleInventory = false;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, bAiming);
	DOREPLIFETIME(APlayerCharacter, BeamEnd);
	DOREPLIFETIME(APlayerCharacter, bBeamEnd);
	DOREPLIFETIME(APlayerCharacter, EquippedWeapon);
	DOREPLIFETIME(APlayerCharacter, bCrouch);
	DOREPLIFETIME(APlayerCharacter, bIsDie);
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

	if (IsLocallyControlled())
	{
		TraceForItems();
	}
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
	ReloadMontage = ReloadMontageSoft.LoadSynchronous();
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;

	InitializeAmmoMap();

	if (HasAuthority())
	{
		EquipWeapon(SpawnDefaultWeapon());
	}

	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}
}

void APlayerCharacter::IncrementOverlappedItemCount(int32 Amount)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

void APlayerCharacter::AimingButtonPressed()
{
	if ((bToggleInventory == true) || (CombatState == EFireState::Reloading))
	{
		return;
	}

	bAimingPressed = true;
	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
	StartAim();
}

void APlayerCharacter::AimingButtonReleased()
{
	bAimingPressed = false;

	if (!bCrouch)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
	}

	StopAim();
}

void APlayerCharacter::CrouchButtonPressed()
{
	bCrouch = true;

	if (!HasAuthority())
	{
		CS_SetCrouch(bCrouch);
	}

	if (!GetCharacterMovement()->IsCrouching() && !GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->bWantsToCrouch = bCrouch;
	}

	GetCharacterMovement()->MaxWalkSpeed = CrouchMovementSpeed;
}

void APlayerCharacter::CrouchButtonReleased()
{
	bCrouch = false;

	if (!HasAuthority())
	{
		CS_SetCrouch(bCrouch);
	}

	if (GetCharacterMovement()->IsCrouching())
	{
		GetCharacterMovement()->bWantsToCrouch = bCrouch;
	}

	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
}

void APlayerCharacter::SprintButtonPressed()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintMovementSpeed;
}

void APlayerCharacter::SprintButtonReleased()
{
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
}

void APlayerCharacter::JumpButtonPressed()
{
	if (bCrouch)
	{
		CrouchButtonReleased();
	}
	else
	{
		ACharacter::Jump();
	}
}

void APlayerCharacter::JumpButtonReleased()
{
	ACharacter::StopJumping();
}

void APlayerCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	FireWeapon();
}

void APlayerCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void APlayerCharacter::InteractiveButtonPressed()
{
	if (IsValid(TraceHitLastFrame))
	{
		GetPickupItem(TraceHitLastFrame);
		TraceHitLastFrame = nullptr;
	}
}

void APlayerCharacter::DropButtonPressed()
{
	DropItem();
}

void APlayerCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
}

void APlayerCharacter::TabButtonPressed()
{
	bToggleInventory = !bToggleInventory;

	UE_LOG(LogTemp, Warning, TEXT("bToggleInventory %d"), bToggleInventory);
	if(AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController()))
	{
		PlayerController->InventoryVisible(bToggleInventory);
	}
}

void APlayerCharacter::FireWeapon()
{
	if (!IsValid(EquippedWeapon) || CombatState != EFireState::Idle || !WeaponHasAmmo())
	{
		return;
	}

	bBeamEnd = false;

	if (IsValid(GetMesh()))
	{
		if(const UStaticMeshSocket* BarrelSocket = EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket"))
		{
			FTransform SocketTransform;
			BarrelSocket->GetSocketTransform(SocketTransform, EquippedWeapon->GetItemMesh());
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

	EquippedWeapon->DecrementAmmo();

	if (IsLocallyControlled())
	{
		RefreshAmmoWidget();
	}

	StartFireTimer();
}

void APlayerCharacter::ReloadWeapon()
{
	if (!IsValid(EquippedWeapon) || CombatState != EFireState::Idle || CarryingAmmo() <= 0 || EquippedWeapon->ClipIsFull())
	{
		return;
	}

	if (bAiming)
	{
		StopAim();
	}

	CombatState = EFireState::Reloading;

	if (HasAuthority())
	{
		SM_ReloadWeapon();
	}
	else
	{
		CS_ReloadWeapon();
	}
}

int32 APlayerCharacter::CarryingAmmo()
{
	if (!IsValid(EquippedWeapon))
	{
		return 0;
	}

	auto AmmoType = EquippedWeapon->GetEAmmoType();

	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType];
	}

	return 0;
}

bool APlayerCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation, float LineDistance)
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
		const FVector End = CrosshairWorldPosition + CrosshairWorldDirection * LineDistance;
		OutHitLocation = End;

		FCollisionQueryParams CollisionParams;
		FCollisionResponseParams CollisionResponese;
		CollisionParams.AddIgnoredActor(this);

		if (EquippedWeapon)
		{
			CollisionParams.AddIgnoredActor(EquippedWeapon);
		}

		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams, CollisionResponese);

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

void APlayerCharacter::TraceForItems()
{
	if (bShouldTraceForItems)
	{
		FHitResult ObjectTraceResult;
		FVector HitLocation;

		if (TraceUnderCrosshairs(ObjectTraceResult, HitLocation, 1000.0f))
		{
			if (AItem* HitItem = Cast<AItem>(ObjectTraceResult.GetActor()))
			{
				HitItem->GetPickupWidget()->SetVisibility(true);

				if (IsValid(TraceHitLastFrame))
				{
					if (TraceHitLastFrame->GetUniqueID() != HitItem->GetUniqueID())
					{
						TraceHitLastFrame->GetPickupWidget()->SetVisibility(false);
					}
				}

				TraceHitLastFrame = HitItem;
			}
			else
			{
				if (IsValid(TraceHitLastFrame))
				{
					TraceHitLastFrame->GetPickupWidget()->SetVisibility(false);
				}
			}
		}
		else
		{
			if (IsValid(TraceHitLastFrame))
			{
				TraceHitLastFrame->GetPickupWidget()->SetVisibility(false);
			}
		}
	}
	else
	{
		if (IsValid(TraceHitLastFrame))
		{
			TraceHitLastFrame->GetPickupWidget()->SetVisibility(false);
			TraceHitLastFrame = nullptr;
		}
	}
}

void APlayerCharacter::EquipWeapon(ARangeWeapon* WeaponToEquip)
{
	if (IsValid(WeaponToEquip))
	{
		if (HasAuthority())
		{
			EquippedWeapon = WeaponToEquip;
			OnRep_AttachWeapon();
		}
		else
		{
			CS_EquipWeapon(WeaponToEquip);
		}
	}
}

void APlayerCharacter::DropItem()
{
	if (IsValid(EquippedWeapon))
	{
		if (HasAuthority())
		{
			SM_DropItem();
			EquippedWeapon = nullptr;
		}
		else
		{
			CS_DropItem();
		}
	}
}

void APlayerCharacter::RefreshAmmoWidget()
{
	if (IsValid(EquippedWeapon))
	{
		int32 CurrAmmo = 0;
		int32 MaxMagazineAmmo = 0;

		if (EquippedWeapon->GetEItemState() == EItemState::Equipped)
		{
			CurrAmmo = EquippedWeapon->GetCurrAmmo();

			MaxMagazineAmmo = CarryingAmmo();
		}
		OnAmmoChanged.Broadcast(CurrAmmo, MaxMagazineAmmo);
	}
}

bool APlayerCharacter::WeaponHasAmmo()
{
	if (IsValid(EquippedWeapon))
	{
		if (EquippedWeapon->GetCurrAmmo() > 0)
		{
			return true;
		}
	}
	return false;
}

void APlayerCharacter::FinishReload()
{
	if (!IsValid(EquippedWeapon))
	{
		return;
	}

	const auto AmmoType = EquippedWeapon->GetEAmmoType();

	if (AmmoMap.Contains(AmmoType))
	{
		int32 CarriedAmmo = AmmoMap[AmmoType];

		const int32 MagazineAmmo = EquippedWeapon->GetEnableReloadAmmo() - EquippedWeapon->GetCurrAmmo();

		if (MagazineAmmo > CarriedAmmo)
		{
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
		}
		else
		{
			EquippedWeapon->ReloadAmmo(MagazineAmmo);
			CarriedAmmo -= MagazineAmmo;
		}

		AmmoMap.Add(AmmoType, CarriedAmmo);
	}

	if (IsLocallyControlled())
	{
		RefreshAmmoWidget();
	}

	CombatState = EFireState::Idle;

	if (bAimingPressed)
	{
		StartAim();
	}
}

void APlayerCharacter::UseItem(FItemInfo ItemInfo)
{
	if (HasAuthority())
	{
		TArray<TSubclassOf<class UGameplayEffect>> ItemUsedEffect = ItemInfo.ItemUseEffect;

		for (auto UseEffect : ItemUsedEffect)
		{
			if (IsValid(UseEffect) && !GetIsDie())
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(UseEffect, 1, EffectContext);

				AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), this->GetAbilitySystemComponent());
			}
		}
	}
	else
	{
		CS_UseItem(ItemInfo);
	}
}

void APlayerCharacter::GetPickupItem(AItem* Item)
{
	if (auto Weapon = Cast<ARangeWeapon>(Item))
	{
		SwapWeapon(Weapon);
	}
	else if (auto Ammo = Cast<AAmmo>(Item))
	{
		GetPickupAmmo(Ammo);
	}
	else if (auto UsableItem = Cast<AUsable>(Item))
	{
		GetPickupUsable(UsableItem);
	}
}

void APlayerCharacter::GetPickupAmmo(AAmmo* Ammo)
{
	if (HasAuthority())
	{
		Ammo->Interact(this);
	}
	else
	{
		CS_GetPickupAmmo(Ammo);
	}

	if (AmmoMap.Contains(Ammo->GetEAmmoType()))
	{
		int32 AmmoCnt = AmmoMap[Ammo->GetEAmmoType()];
		AmmoCnt += Ammo->GetItemCnt();
		AmmoMap[Ammo->GetEAmmoType()] = AmmoCnt;
	}

	if (EquippedWeapon->GetEAmmoType() == Ammo->GetEAmmoType())
	{
		RefreshAmmoWidget();
	}
}

void APlayerCharacter::GetPickupUsable(AUsable* Usable)
{
	if (AShootPlayerController* PlayerController = Cast<AShootPlayerController>(GetController()))
	{
		PlayerController->AddItemToInventoryWidget(Usable->GetItemInfo());
	}

	DestroyUsable(Usable);
}

void APlayerCharacter::OnRep_AttachWeapon()
{
	if (IsValid(EquippedWeapon))
	{
		if (EquippedWeapon->GetEItemState() == EItemState::PickUp)
		{
			EquippedWeapon->SetItemState(EItemState::Equipped);
			EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightHandSocket"));
		}

		if (IsLocallyControlled())
		{
			RefreshAmmoWidget();
		}
	}
}
void APlayerCharacter::OnRep_Die()
{
	if (DeadMontage.Num() > 0)
	{
		int32 MontageIndex = FMath::RandRange(0, DeadMontage.Num() - 1);
		UAnimationAsset* DeathMontage = DeadMontage[MontageIndex];
		GetMesh()->PlayAnimation(DeathMontage, false);
	}

	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Ragdoll")),true);
}

void APlayerCharacter::CS_FireWeapon_Implementation(const FVector& BeamEndParam, bool bBeamEndParam)
{
	BeamEnd = BeamEndParam;
	bBeamEnd = bBeamEndParam;

	SM_FireWeapon(BeamEnd, bBeamEnd);
}

void APlayerCharacter::SM_FireWeapon_Implementation(const FVector& BeamEndParam, bool bBeamEndParam)
{
	PlayFireSound();
	SendBullet(BeamEndParam, bBeamEndParam);
	PlayGunFireMontage();
}

void APlayerCharacter::CS_HitDamage_Implementation(FHitResult HitResult, TSubclassOf<class UGameplayEffect> DamageEffect)
{
	SM_HitDamage(HitResult, DamageEffect);
}

void APlayerCharacter::SM_HitDamage_Implementation(FHitResult HitResult, TSubclassOf<class UGameplayEffect> DamageEffect)
{
	if (AActor* HitActor = HitResult.GetActor())
	{
		///////////////////////////////현재는 플레이어 끼리 데미지 입히는 구조////////////////////
		if (APlayerCharacter* HitActorPlayer = Cast<APlayerCharacter>(HitActor))
		{
			if (HitActorPlayer->GetIsDie())
			{
				return;
			}

			if (IsValid(DamageEffect))
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(DamageEffect, 1, EffectContext);

				AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), HitActorPlayer->GetAbilitySystemComponent());
			}
		}
		else if (AAICharacter* HitActorAI = Cast<AAICharacter>(HitActor))
		{
			if (HitActorAI->GetIsDie())
			{
				return;
			}

			if (IsValid(DamageEffect) && !HitActorAI->GetIsDie())
			{
				FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(DamageEffect, 1, EffectContext);

				AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), HitActorAI->GetAbilitySystemComponent());
			
				if(IsValid(AttributeSetComp))
					HitActorAI->ShowHitNumber(AttributeSetComp->GetFireDamage(), HitResult.Location);
			}

		}
	}
}

void APlayerCharacter::CS_SetAiming_Implementation(bool WantsToAim)
{
	bAiming = WantsToAim;
}

void APlayerCharacter::CS_SetCrouch_Implementation(bool WantsToCrouch)
{
	bCrouch = WantsToCrouch;
}

void APlayerCharacter::CS_EquipWeapon_Implementation(ARangeWeapon* WeaponToEquip)
{
	EquipWeapon(WeaponToEquip);
}

void APlayerCharacter::CS_DropItem_Implementation()
{
	DropItem();
}

void APlayerCharacter::SM_DropItem_Implementation()
{
	if (IsValid(EquippedWeapon))
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);
		EquippedWeapon->SetItemState(EItemState::PickUp);
	}

	if (IsLocallyControlled())
	{
		RefreshAmmoWidget();
	}
}

void APlayerCharacter::CS_ReloadWeapon_Implementation()
{
	SM_ReloadWeapon();
}

void APlayerCharacter::SM_ReloadWeapon_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(ReloadMontage);
		AnimInstance->Montage_JumpToSection(EquippedWeapon->GetReloadMontageSection());
	}
}

void APlayerCharacter::CS_GetPickupAmmo_Implementation(AAmmo* Ammo)
{
	GetPickupAmmo(Ammo);
}

void APlayerCharacter::CS_DestroyUsable_Implementation(AUsable* Usable)
{
	DestroyUsable(Usable);
}

void APlayerCharacter::CS_UseItem_Implementation(FItemInfo ItemInfo)
{
	UseItem(ItemInfo);
}

void APlayerCharacter::Die()
{
	if (HasAuthority())
	{
		bIsDie = true;
		OnRep_Die();
	}

	GetCharacterMovement()->DisableMovement();
}

bool APlayerCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation, 30000.0f);

	if (bCrosshairHit)
	{
		OutBeamLocation = CrosshairHitResult.Location;
	}

	FCollisionQueryParams CollisionParams;
	FCollisionResponseParams CollisionResponese;
	CollisionParams.AddIgnoredActor(this);

	if (EquippedWeapon)
	{
		CollisionParams.AddIgnoredActor(EquippedWeapon);
	}

	FHitResult WeaponTraceHit;
	const FVector WeaponTraceStart = MuzzleSocketLocation;
	const FVector StartToEnd = OutBeamLocation - MuzzleSocketLocation;
	const FVector WeaponTraceEnd = MuzzleSocketLocation + StartToEnd * 1.25f;

	GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_EngineTraceChannel2, CollisionParams, CollisionResponese);

	if (WeaponTraceHit.bBlockingHit)
	{
		OutBeamLocation = WeaponTraceHit.Location;
		HitDamage(WeaponTraceHit, AttackDamageEffect);
		return true;
	}

	return false;
}

ARangeWeapon* APlayerCharacter::SpawnDefaultWeapon()
{
	if (IsValid(DefaultWeaponClass))
	{
		return GetWorld()->SpawnActor<ARangeWeapon>(DefaultWeaponClass);
	}

	return nullptr;
}

void APlayerCharacter::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::Ammo9mm, StartingAmmo9mm);
	AmmoMap.Add(EAmmoType::AmmoAR, StartingAmmoAR);
}

void APlayerCharacter::PlayFireSound()
{
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 0.0f, Tags::Noise_Tag);
}

void APlayerCharacter::SendBullet(const FVector& BeamEndParam, bool bBeamEndParam)
{
	const UStaticMeshSocket* BarrelSocket = EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");

	if (IsValid(BarrelSocket))
	{
		FTransform SocketTransform;
		BarrelSocket->GetSocketTransform(SocketTransform, EquippedWeapon->GetItemMesh());

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

void APlayerCharacter::PlayGunFireMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}

	StartCrosshairBulletFire();
}

void APlayerCharacter::StartAim()
{
	bAiming = true;

	if (!HasAuthority())
	{
		CS_SetAiming(bAiming);
	}
}

void APlayerCharacter::StopAim()
{
	bAiming = false;

	if (!HasAuthority())
	{
		CS_SetAiming(bAiming);
	}
}

void APlayerCharacter::SwapWeapon(ARangeWeapon* Weapon)
{
	if (IsValid(EquippedWeapon))
	{
		DropItem();
	}

	EquipWeapon(Weapon);
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

void APlayerCharacter::StartFireTimer()
{
	CombatState = EFireState::FireInProgress;
	GetWorldTimerManager().SetTimer(AutoFireTimer, this, &APlayerCharacter::AutoFireReset, AutomaticFireRate, false);
}

void APlayerCharacter::DestroyUsable(AUsable* Usable)
{
	if (HasAuthority())
	{
		Usable->Interact(this);
	}
	else
	{
		CS_DestroyUsable(Usable);
	}
}

void APlayerCharacter::AutoFireReset()
{
	CombatState = EFireState::Idle;

	if (WeaponHasAmmo())
	{
		if (bFireButtonPressed)
		{
			FireWeapon();
		}
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


void APlayerCharacter::HitDamage(FHitResult HitResult, TSubclassOf<class UGameplayEffect> DamageEffect)
{
	if (HasAuthority())
	{
		SM_HitDamage(HitResult, DamageEffect);
	}
	else
	{
		CS_HitDamage(HitResult, DamageEffect);
	}
}