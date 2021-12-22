// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LeageOfShooter/Item/Item.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAGEOFSHOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void AddItemInfo(FItemInfo ItemInfo);

	UFUNCTION(BlueprintCallable)
	void RemoveItemInfo(int32 RemoveIndex);

	UFUNCTION(BlueprintCallable)
	void DropItem(int32 DropIndex);

	UFUNCTION(BlueprintCallable)
	void UseItem(int32 UseIndex);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = InputComponent, meta = (AllowPrivateAccess = "true"))
	TArray<FItemInfo> InventoryItemArray;
		
	UPROPERTY(Transient)
	class APlayerCharacter* OwnerCharacter;
};
