// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeageOfShooter/Character/BaseCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Components/WidgetComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/SphereComponent.h"
#include "AICharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChanged, float, CurHealth, float, MaxHealth);

UCLASS()
class LEAGEOFSHOOTER_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AAICharacter();
	virtual void Tick(float DeltaTime) override;

	void VisibleHealthBar(bool bVisible);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumber(float Damage, FVector HitLocation);

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	void DestroyHitNumber(UUserWidget* HitNumber);
	void UpdateHitNumbers();

	void AggroEnemy();
	void LightAttackEnemy();

	void EnableMeleeSphere();
	void DisableMeleeSphere();

	FORCEINLINE UWidgetComponent* GetHealthBarWidget() const { return HealthBarWidget; }
	FORCEINLINE class UAnimMontage* GetLightAttackMontage() const { return CurrLightAttackMontage; }
	FORCEINLINE bool GetIsDie() const { return bIsDie; }
protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void CS_VisibleHealthBar(bool bVisible);
	UFUNCTION(NetMulticast, Reliable)
	void SM_VisibleHealthBar(bool bVisible);

	UFUNCTION(NetMulticast, Reliable)
	void SM_MultiAnimation(UAnimMontage* Montage, FName SectionName);

	UFUNCTION(NetMulticast, Reliable)
	void SM_EnemyDie();

	virtual void Die() override;

	virtual void CallHealthChanged() override;

	virtual void ActorDestory() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void Setup_Stimulus();

public:
	UPROPERTY(EditAnywhere, Category = Combat)
	TSoftObjectPtr<UAnimMontage> AggroMontageSoft;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<TSoftObjectPtr<UAnimMontage>> LightAttackMontageSoft;

protected:
	UPROPERTY(BlueprintAssignable)
	FHealthChanged OnHealthChanged;

private:
	UPROPERTY(Transient)
	class UAIPerceptionStimuliSourceComponent* Stimulus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Character", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Character", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* MeleeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TMap<UUserWidget*, FVector> HitNumbers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float HitNumberDestroyTime;

	UPROPERTY(Transient)
	class UAnimMontage* AggroMontage;

	UPROPERTY(Transient)
	UAnimMontage* CurrLightAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEnemysWeapon> DefaultWeaponClass;

	UPROPERTY(Transient)
	AEnemysWeapon* WeaponMesh;

	UPROPERTY(Transient)
	TArray<UAnimMontage*> LightAttackMontage;

	bool bIsDie;
	FName AggroSectionName;
	FName LightAttackSectionName;
};
