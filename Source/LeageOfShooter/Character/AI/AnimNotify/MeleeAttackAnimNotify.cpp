// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAttackAnimNotify.h"
#include "LeageOfShooter/Character/AI/AICharacter.h"

void UMeleeAttackAnimNotify::NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!IsValid(MeshComp))
	{
		return;
	}

	if(AAICharacter* Owner = Cast<AAICharacter>(MeshComp->GetOwner()))
	{
		Owner->EnableMeleeSphere();
	}
}

void UMeleeAttackAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp))
		return;

	if (AAICharacter* Owner = Cast<AAICharacter>(MeshComp->GetOwner()))
	{
		Owner->DisableMeleeSphere();
	}
}