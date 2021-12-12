// Fill out your copyright notice in the Description page of Project Settings.


//////////Fire은 그냥 일반 기능으로 냅둘 예정/////////
///// 특수 액티브 스킬 들만 따로 GameAbility로 구현 예정/////////
/// 예시를 위해 냅두는중/////
#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "GA_Fire.generated.h"

/**
 * 
 */
UCLASS()
class LEAGEOFSHOOTER_API UGA_Fire : public UBaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Fire();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY(Transient)
	class APlayerCharacter* OwnerChar;


};
