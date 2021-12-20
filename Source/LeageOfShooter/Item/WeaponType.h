#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SMG UMETA(DisplayName = "SMG"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle  UMETA(DisplayName = "Rifle"),
};
