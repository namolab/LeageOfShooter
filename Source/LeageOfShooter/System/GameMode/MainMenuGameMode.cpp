// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "LeageOfShooter/System/Beacon/ShooterBeaconHostObject.h"
#include "OnlineBeaconHost.h"


AMainMenuGameMode::AMainMenuGameMode()
{
	HostObject = nullptr;
	Host = nullptr;
}

bool AMainMenuGameMode::CreateHostBeacon()
{
	Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass());
	if (Host)
	{
		if (Host->InitHost())
		{
			Host->PauseBeaconRequests(false);
			HostObject = GetWorld()->SpawnActor<AShooterBeaconHostObject>(AShooterBeaconHostObject::StaticClass());
			if (HostObject)
			{
				Host->RegisterHost(HostObject);
				return true;
			}
		}
	}

	return false;
}

AShooterBeaconHostObject* AMainMenuGameMode::GetBeaconHost()
{
	return HostObject;
}
