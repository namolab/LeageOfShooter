// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ShooterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class LEAGEOFSHOOTER_API UShooterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "ShooterDataCollection")
	TSoftObjectPtr<UDataTable> CharacterTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "ShooterDataCollection")
	TSoftObjectPtr<UDataTable> CharacterModelTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "ShooterDataCollection")
	TSoftObjectPtr<UDataTable> NoticeTable;

	UPROPERTY(EditDefaultsOnly, Category = "ShooterDataCollection")
	TSoftObjectPtr<UDataTable> WeaponTable;
};
