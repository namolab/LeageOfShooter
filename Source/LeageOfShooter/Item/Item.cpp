// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	RootComponent = ItemMesh;

	CollisionBox = CreateDefaultSubobject<USphereComponent>("CollisionBox");
	CollisionBox->SetupAttachment(ItemMesh);

	AreaSphere = CreateDefaultSubobject<USphereComponent>("Area Sphere");
	AreaSphere->SetupAttachment(ItemMesh);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>("PickupWidget");
	PickupWidget->SetupAttachment(GetRootComponent());

	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	ItemInfo.ItemName = FName(TEXT("Default"));
	ItemState = EItemState::PickUp;
	ItemCnt = 1;
	ItemInfo.ItemClass = StaticClass();

	bReplicates = true;
	SetReplicateMovement(true);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SetItemProperty();
	}

	PickupWidget->SetVisibility(false);
	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

void AItem::DisablePhysics()
{
	ItemMesh->SetSimulatePhysics(false);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupWidget->SetVisibility(false);
}

void AItem::DisableAllPhysics()
{
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupWidget->SetVisibility(false);
}

void AItem::EnablePhysics()
{
	ItemMesh->SetSimulatePhysics(true);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			PlayerCharacter->IncrementOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			PlayerCharacter->IncrementOverlappedItemCount(-1);
		}
	}
}


void AItem::SetItemProperty()
{
	switch (ItemState)
	{
		case EItemState::PickUp:
			EnablePhysics();
			ItemMesh->SetVisibility(true);
			break;
		case EItemState::PickedUp:
			DisableAllPhysics();
			ItemMesh->SetVisibility(false);
			break;
		case EItemState::Equipped:
			DisablePhysics();
			ItemMesh->SetVisibility(true);
			break;
		default:
			break;
	}
}

void AItem::OnConstruction()
{
}

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperty();
}

