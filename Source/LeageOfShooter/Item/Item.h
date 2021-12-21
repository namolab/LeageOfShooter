// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Item.generated.h"


UENUM(BlueprintType)
enum class EItemState : uint8
{
	PickUp UMETA(DisplayName = "PickUp"),
	PickedUp  UMETA(DisplayName = "PickedUp"),
	Equipped  UMETA(DisplayName = "Equipped")
};

UENUM(BlueprintType)
enum class EItemGrade : uint8
{
	Common  UMETA(DisplayName = "Common"),
	UnCommon  UMETA(DisplayName = "UnCommon"),
	Rare  UMETA(DisplayName = "Rare"),
	Unique  UMETA(DisplayName = "Unique")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Item UMETA(DisplayName = "Item"),
	Weapon  UMETA(DisplayName = "Weapon"),
};

UENUM(BlueprintType)
enum class EItemEffect : uint8
{
	WeaponDamage  UMETA(DisplayName = "WeaponDamage"),
	IncreaseHealth  UMETA(DisplayName = "IncreaseHealth"),
	IncreaseMana  UMETA(DisplayName = "IncreaseMana"),
	IncreaseDamage  UMETA(DisplayName = "IncreaseDamage"),
	IncreaseAmmo  UMETA(DisplayName = "IncreaseAmmo"),
};

USTRUCT(BlueprintType)
struct  FItemStat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	EItemEffect StatName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	EItemGrade StatGrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	float StatValue;
};

USTRUCT(BlueprintType)
struct  FItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	FName ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	class UTexture2D* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	EItemGrade ItemGrade;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	TArray<FItemStat> ItemStat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	TArray<TSubclassOf<class UGameplayEffect>> ItemUseEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info")
	TSubclassOf<class AItem> ItemClass;
};

UCLASS()
class LEAGEOFSHOOTER_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	void DisablePhysics();
	void DisableAllPhysics();
	void EnablePhysics();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void SetItemProperty();

	virtual void OnConstruction();

public:	
	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE UStaticMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE USphereComponent* GetCollisionBox() const { return CollisionBox; }
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE EItemState GetEItemState() const { return ItemState; }
	FORCEINLINE int32 GetItemCnt() const { return ItemCnt; }
	FORCEINLINE FItemInfo GetItemInfo() const { return ItemInfo; }

	void SetItemState(EItemState State);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	FItemInfo ItemInfo;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* PickupWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemCnt;
};
