// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeWeapon.h"

ARangeWeapon::ARangeWeapon()
	:Super()
{
	EnableReloadAmmo = 30;
	CurrAmmo = EnableReloadAmmo;
	RangeWeaponType = EWeaponType::Rifle;
	AmmoType = EAmmoType::AmmoAR;
	ReloadMontageSection= FName(TEXT("Reload_Rifle"));
}

// Called when the game starts or when spawned
void ARangeWeapon::BeginPlay()
{
	Super::BeginPlay();
	OnConstruction();
}

void ARangeWeapon::OnConstruction()
{
	Super::OnConstruction();
	const FString WeaponTablePath = TEXT("DataTable'/Game/Data/WeaponDataTable.WeaponDataTable'");
	UDataTable* WeaponTableObject = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponTablePath));

	if (IsValid(WeaponTableObject))
	{
		FWeaponDataTable* WeaponDataRow = nullptr;

		switch (RangeWeaponType)
		{
			case EWeaponType::Pistol:
				WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Pistol"), TEXT(""));
				break;
			case EWeaponType::Rifle:
				WeaponDataRow = WeaponTableObject->FindRow<FWeaponDataTable>(FName("Rifle"), TEXT(""));
				break;
			default:
				break;
		}

		if (WeaponDataRow != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnConstruction"));
			AmmoType = WeaponDataRow->AmmoType;
			CurrAmmo = WeaponDataRow->WeaponAmmo;
			EnableReloadAmmo = WeaponDataRow->MagazineCapacity;
			GetItemMesh()->SetStaticMesh(WeaponDataRow->ItemMesh.LoadSynchronous());
			ItemInfo.ItemDescription = WeaponDataRow->ItemDescription;
			ItemInfo.ItemImage = WeaponDataRow->ItemImage;
			ItemInfo.ItemName = WeaponDataRow->ItemName;
		}

	}
}

void ARangeWeapon::DecrementAmmo()
{
	if (CurrAmmo > 0)
	{
		--CurrAmmo;
	}
}

void ARangeWeapon::ReloadAmmo(int32 Ammo)
{
	CurrAmmo += Ammo;
}

bool ARangeWeapon::ClipIsFull()
{
	return CurrAmmo >= EnableReloadAmmo;
}