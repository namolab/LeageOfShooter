// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeageOfShooter/Item/Item.h"
#include "LeageOfShooter/Item/AmmoType.h"
#include "Ammo.generated.h"

/**
 * 
 */
UCLASS()
class LEAGEOFSHOOTER_API AAmmo : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAmmo();
	FORCEINLINE EAmmoType GetEAmmoType() const { return AmmoType; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Info", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;
};
