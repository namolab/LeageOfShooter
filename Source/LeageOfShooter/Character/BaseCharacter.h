// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "LeageOfShooter/Attribute/BaseAttributeSet.h"
#include "LeageOfShooter/Ability/BaseGameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "BaseCharacter.generated.h"

UCLASS()
class LEAGEOFSHOOTER_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable)
	virtual void InitializeAbility();

	UFUNCTION(BlueprintCallable)
	virtual void InitializeAttribute();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyCharacter")
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyCharacter")
	class UBaseAttributeSet* AttributeSetComp;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	TArray<TSubclassOf<class UBaseGameplayAbility>> DefualtAbility;

};
