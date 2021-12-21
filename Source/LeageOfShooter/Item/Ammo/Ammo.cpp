// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"

AAmmo::AAmmo()
	:Super()
{
	AmmoType = EAmmoType::AmmoAR;
}

void AAmmo::Interact(class APlayerCharacter* Player)
{
	Destroy();
}