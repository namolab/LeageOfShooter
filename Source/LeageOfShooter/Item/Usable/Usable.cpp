// Fill out your copyright notice in the Description page of Project Settings.


#include "Usable.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "LeageOfShooter/Character/Player/Controller/ShootPlayerController.h"

AUsable::AUsable()
	: Super()
{

}

void AUsable::BeginPlay()
{
	Super::BeginPlay();
}

void AUsable::Interact(APlayerCharacter* Player)
{
	Destroy();
}