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
	ABaseCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual void InitializeAbility();

	UFUNCTION(BlueprintCallable)
	virtual void InitializeAttribute();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

protected:
	virtual void BeginPlay() override;
	void OnHealthChangedFunc(const FOnAttributeChangeData& Data);
	virtual void Die();

	virtual void CallHealthChanged();
	virtual void ActorDestory();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyCharacter")
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MyCharacter")
	class UBaseAttributeSet* AttributeSetComp;


public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	TArray<TSubclassOf<class UBaseGameplayAbility>> DefualtAbility;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<TSoftObjectPtr<UAnimationAsset>> DeadMontageSoft;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	TSubclassOf<class UGameplayEffect> AttackDamageEffect;

	UPROPERTY(Transient)
	TArray<UAnimationAsset*> DeadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MyCharacter")
	FName DeadSectionName;

};
