// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishReloadNotify.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"

void UFinishReloadNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp))
	{
		return;
	}

	if (APlayerCharacter* Owner = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		Owner->FinishReload();
	}

}