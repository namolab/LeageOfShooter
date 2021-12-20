// Fill out your copyright notice in the Description page of Project Settings.


#include "KrakenAnimInstance.h"
#include "LeageOfShooter/Character/AI/KrakenAICharacter.h"

UKrakenAnimInstance::UKrakenAnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;
}

void UKrakenAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	KrakenAI = Cast<AKrakenAICharacter>(TryGetPawnOwner());
	
	if (!IsValid(KrakenAI))
	{
		return;
	}

	FVector Velocity = KrakenAI->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	FRotator KrakenRot = KrakenAI->GetActorRotation();
	Direction = CalculateDirection(Velocity, KrakenRot);
}
