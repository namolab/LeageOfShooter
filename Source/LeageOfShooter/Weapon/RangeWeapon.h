// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LEAGEOFSHOOTER_API ARangeWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ARangeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
