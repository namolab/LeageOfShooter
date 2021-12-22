// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LeageOfShooter/Item/Weapon/EnemysWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "LeageOfShooter/Character/AI/Controller/BaseAIController.h"
#include "LeageOfShooter/Character/AI/BlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidget");
	HealthBarWidget->SetupAttachment(GetRootComponent());

	MeleeSphere = CreateDefaultSubobject<USphereComponent>("WeaponMesh");
	MeleeSphere->SetupAttachment(GetMesh());

	HitNumberDestroyTime = 0.7f;
	bUseControllerRotationYaw = false;
	AggroSectionName = FName(TEXT("AggroSection"));
	LightAttackSectionName = FName(TEXT("LightAttack"));

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	Setup_Stimulus();
}

void AAICharacter::Setup_Stimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));

	if (IsValid(Stimulus))
	{
		Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
		Stimulus->RegisterWithPerceptionSystem();
	}
}

void AAICharacter::CallHealthChanged()
{
	Super::CallHealthChanged();
	OnHealthChanged.Broadcast(AttributeSetComp->GetHealth(), AttributeSetComp->GetMaxHealth());
}

void AAICharacter::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || GetIsDie())
	{
		return;
	}

	if (IsValid(OtherActor) && IsValid(AttackDamageEffect))
	{
		if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(AttackDamageEffect, 1, EffectContext);

			AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), PlayerCharacter->GetAbilitySystemComponent());
		}
	}
}

void AAICharacter::ActorDestory()
{
	Super::ActorDestory();
}

void AAICharacter::StoreHitNumber(UUserWidget* HitNumber, FVector Location)
{
	HitNumbers.Add(HitNumber, Location);

	FTimerHandle HitNumberTimer;
	FTimerDelegate HitNumberDelegate = FTimerDelegate::CreateUObject(this, &AAICharacter::DestroyHitNumber, HitNumber);

	GetWorld()->GetTimerManager().SetTimer(HitNumberTimer, HitNumberDelegate, HitNumberDestroyTime, false);
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	AggroMontage = AggroMontageSoft.LoadSynchronous();
	for (int32 i = 0; i < LightAttackMontageSoft.Num(); i++)
	{
		LightAttackMontage.Add(LightAttackMontageSoft[i].LoadSynchronous());
	}

	HealthBarWidget->SetVisibility(false);

	if (IsValid(DefaultWeaponClass))
	{
		WeaponMesh = GetWorld()->SpawnActor<AEnemysWeapon>(DefaultWeaponClass);
		WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Enemy_Weapon"));
	}

	MeleeSphere->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnSphereOverlap);
}


void AAICharacter::CS_VisibleHealthBar_Implementation(bool bVisible)
{
	SM_VisibleHealthBar(bVisible);
}

void AAICharacter::SM_VisibleHealthBar_Implementation(bool bVisible)
{
	HealthBarWidget->SetVisibility(bVisible);
}

void AAICharacter::VisibleHealthBar(bool bVisible)
{
	if (HasAuthority())
	{
		SM_VisibleHealthBar(bVisible);
	}
	else
	{
		CS_VisibleHealthBar(bVisible);
	}
}

void AAICharacter::Die()
{
	Super::Die();

	bIsDie = true;

	if (ABaseAIController* Cont = Cast<ABaseAIController>(GetController()))
	{
		Cont->GetBlackboard()->SetValueAsBool(BBKeys::EnemyDie, true);
		Cont->GetBlackboard()->SetValueAsObject(BBKeys::TargetPlayer, nullptr);
		Cont->GetBlackboard()->SetValueAsBool(BBKeys::CanSeePlayer, false);
	}

	VisibleHealthBar(false);

	if (HasAuthority())
	{
		SM_EnemyDie();
	}

	DetachFromControllerPendingDestroy();
	GetCharacterMovement()->SetAvoidanceEnabled(false);
	GetCharacterMovement()->DisableMovement();

	FTimerHandle DeadShootTimer;
	GetWorldTimerManager().SetTimer(DeadShootTimer, this, &AAICharacter::ActorDestory, 5.0f, false);
}

void AAICharacter::DestroyHitNumber(UUserWidget* HitNumber)
{
	HitNumbers.Remove(HitNumber);
	HitNumber->RemoveFromParent();
}

void AAICharacter::UpdateHitNumbers()
{
	for (auto& HitPair : HitNumbers)
	{
		UUserWidget* HitNumber = HitPair.Key;
		const FVector Location = HitPair.Value;
		FVector2D ScreenPos;

		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), Location, ScreenPos);
		HitNumber->SetPositionInViewport(ScreenPos);
	}
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHitNumbers();
}

void AAICharacter::AggroEnemy()
{
	if (HasAuthority())
	{
		SM_MultiAnimation(AggroMontage, AggroSectionName);
	}
}

void AAICharacter::LightAttackEnemy()
{
	if (LightAttackMontage.Num() > 0)
	{
		int32 MontageIndex = FMath::RandRange(0, LightAttackMontage.Num() - 1);
		CurrLightAttackMontage = LightAttackMontage[MontageIndex];

		if (HasAuthority())
		{
			SM_MultiAnimation(CurrLightAttackMontage, LightAttackSectionName);
		}
	}
}

void AAICharacter::EnableMeleeSphere()
{
	MeleeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAICharacter::DisableMeleeSphere()
{
	MeleeSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAICharacter::SM_MultiAnimation_Implementation(UAnimMontage* Montage, FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AAICharacter::SM_EnemyDie_Implementation()
{
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Ragdoll")), true);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	if (DeadMontage.Num() > 0)
	{
		int32 MontageIndex = FMath::RandRange(0, DeadMontage.Num() - 1);
		UAnimationAsset* DeathMontage = DeadMontage[MontageIndex];
		GetMesh()->PlayAnimation(DeathMontage, false);
	}
}