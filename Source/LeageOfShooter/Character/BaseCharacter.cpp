// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);

	AttributeSetComp = CreateDefaultSubobject<UBaseAttributeSet>("Attributes");
	AbilitySystemComp->AddAttributeSetSubobject(AttributeSetComp);

	DeadSectionName = FName(TEXT("Dead"));
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AbilitySystemComp && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInputID", static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));
		AbilitySystemComp->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void ABaseCharacter::InitializeAbility()
{
	if (HasAuthority() && AbilitySystemComp)
	{
		for (TSubclassOf<UBaseGameplayAbility>& AbilityClass : DefualtAbility)
		{
			if (AbilityClass)
			{
				AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, static_cast<int32>(AbilityClass.GetDefaultObject()->AbilityInputId),this));
			}
		}
	}
}

void ABaseCharacter::InitializeAttribute()
{
	if (AbilitySystemComp)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect>& EffectClass : DefaultAttributeEffect)
		{
			if (EffectClass)
			{
				FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(EffectClass, 1, EffectContext);

				if (SpecHandle.IsValid())
				{
					FActiveGameplayEffectHandle GEHandle = AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
		AttributeSetComp->Initialize(this);
	}
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComp->InitAbilityActorInfo(this, this);

	/////////////////////////Server/////////////////////////
	InitializeAbility();
	InitializeAttribute();
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AbilitySystemComp->InitAbilityActorInfo(this, this);

	////////Client///////

	InitializeAttribute();

	if (AbilitySystemComp && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EAbilityInputID", static_cast<int32>(EAbilityInputID::Confirm), static_cast<int32>(EAbilityInputID::Cancel));
		AbilitySystemComp->BindAbilityActivationToInputComponent(InputComponent,Binds);
	}

}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < DeadMontageSoft.Num(); i++)
	{
		DeadMontage.Add(DeadMontageSoft[i].LoadSynchronous());
	}

	if (AbilitySystemComp)
	{
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSetComp->GetHealthAttribute()).AddUObject(this, &ABaseCharacter::OnHealthChangedFunc);
	}
}

void ABaseCharacter::OnHealthChangedFunc(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0.0f)
	{
		if (IsValid(GEngine))
		{
			GEngine->AddOnScreenDebugMessage(1, 6.0f, FColor::White, FString(TEXT("Die")));
		}

		Die();
	}

	CallHealthChanged();
}

void ABaseCharacter::Die()
{

}

void ABaseCharacter::CallHealthChanged()
{
}

void ABaseCharacter::ActorDestory()
{
	Destroy();
}