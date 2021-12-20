// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LeageOfShooter/Item/Item.h"
#include "LeageOfShooter/Item/AmmoType.h"
#include "LeageOfShooter/Item/WeaponType.h"
#include "Engine/DataTable.h"
#include "RangeWeapon.generated.h"


USTRUCT(BlueprintType)
struct FWeaponDataTable : public FTableRowBase
{	
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> ItemImage;
};


UCLASS()
class LEAGEOFSHOOTER_API ARangeWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ARangeWeapon();

	void DecrementAmmo();
	void ReloadAmmo(int32 Ammo);
	bool ClipIsFull();

	FORCEINLINE EWeaponType GetEWeaponType() const { return RangeWeaponType; }
	FORCEINLINE EAmmoType GetEAmmoType() const { return AmmoType; }
	FORCEINLINE int32 GetCurrAmmo() const { return CurrAmmo; }
	FORCEINLINE int32 GetEnableReloadAmmo() const { return EnableReloadAmmo; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 CurrAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	int32 EnableReloadAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EWeaponType RangeWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FName ReloadMontageSection;
};
