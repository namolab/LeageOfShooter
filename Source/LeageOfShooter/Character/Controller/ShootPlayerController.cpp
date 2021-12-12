// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootPlayerController.h"
#include "LeageOfShooter/Character/Player/PlayerCharacter.h"
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
