// Fill out your copyright notice in the Description page of Project Settings.


#include "TrollAnimInstance.h"
#include "LeageOfShooter/Character/AI/TrollAI.h"


UTrollAnimInstance::UTrollAnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;
}

void UTrollAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	TrollAI = Cast<ATrollAI>(TryGetPawnOwner());

	if (!IsValid(TrollAI))
	{
		return;
	}

	FVector Velocity = TrollAI->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();
	
	FRotator TrollRot = TrollAI->GetActorRotation();
	Direction = CalculateDirection(Velocity, TrollRot);
}
