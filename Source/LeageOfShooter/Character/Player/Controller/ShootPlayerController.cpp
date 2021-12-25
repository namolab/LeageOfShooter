// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootPlayerController.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

AShootPlayerController::AShootPlayerController()
	: Super()
{

}

void AShootPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	UpdateListener();
}

void AShootPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		if (MainWidgetClass != nullptr)
		{
			MainWidget = CreateWidget<UUserWidget>(this, MainWidgetClass);
			MainWidget->AddToViewport();
		}

		if (InventoryWidgetClass != nullptr)
		{
			InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AShootPlayerController::InventoryVisible(bool Visible)
{
	if (IsValid(InventoryWidget))
	{
		if (Visible)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			FInputModeGameAndUI InputModeGameAndUI;
			SetInputMode(InputModeGameAndUI);
		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			FInputModeGameOnly InputModeGameOnly;
			SetInputMode(InputModeGameOnly);
		}

		bShowMouseCursor = Visible;

		ToggleInventory(Visible);
	}
}

void AShootPlayerController::UpdateListener()
{
	if (IsValid(GetWorld()) && IsLocalController())
	{
		if (APlayerCameraManager* camManager = this->PlayerCameraManager)
		{
			APlayerCharacter* OwnerPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

			if (IsValid(OwnerPlayer) && IsValid(OwnerPlayer->GetMesh()))
			{
				FRotator PlayerRot = OwnerPlayer->GetMesh()->GetComponentRotation();
				FRotator CamRot = camManager->GetCameraRotation();
				
				PlayerRot = -1 * PlayerRot;

				FRotator ListenerRot = FRotator(FQuat(PlayerRot) * FQuat(CamRot));
				FRotator ListenerNewRot = FRotator(ListenerRot.Pitch, ListenerRot.Yaw+90.0f, ListenerRot.Roll);

				this->SetAudioListenerOverride(OwnerPlayer->GetMesh(), FVector::ZeroVector, ListenerNewRot);
			}
		}
	}
}
