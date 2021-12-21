// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseController.h"
#include "LeageOfShooter/Item/Usable/Usable.h"
#include "ShootPlayerController.generated.h"


UCLASS()
class LEAGEOFSHOOTER_API AShootPlayerController : public ABaseController
{
	GENERATED_BODY()
	
public:
	AShootPlayerController();

	virtual void PlayerTick(float DeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddItemToInventoryWidget(FItemInfo ItemData);

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleInventory(bool bToggleVisible);

	void InventoryVisible(bool Visible);

protected:
	virtual void BeginPlay() override;

private:
	void UpdateListener();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<class UUserWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	class UUserWidget* InventoryWidget;

private:
	UPROPERTY(Transient)
	class UUserWidget* MainWidget;

};
