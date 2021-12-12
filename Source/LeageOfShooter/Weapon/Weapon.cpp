// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
	RootComponent = WeaponMesh;

	CollisionBox = CreateDefaultSubobject<USphereComponent>("CollisionBox");
	CollisionBox->SetupAttachment(WeaponMesh);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponMesh->SetSimulatePhysics(true);
}

void AWeapon::DisablePhysics()
{
	WeaponMesh->SetSimulatePhysics(false);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
//void AWeapon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

