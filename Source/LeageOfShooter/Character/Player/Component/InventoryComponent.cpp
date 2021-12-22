// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItemInfo(FItemInfo ItemInfo)
{
	InventoryItemArray.Add(ItemInfo);
}

void UInventoryComponent::RemoveItemInfo(int32 RemoveIndex)
{
	InventoryItemArray.RemoveAt(RemoveIndex);
}

void UInventoryComponent::DropItem(int32 DropIndex)
{

}

void UInventoryComponent::UseItem(int32 UseIndex)
{
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->UseItem(InventoryItemArray[UseIndex]);
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter"));
	}
}

