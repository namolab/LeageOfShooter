// Fill out your copyright notice in the Description page of Project Settings.


#include "LeageOfShooterGameInstance.h"
//#include "../Plugins/AdvancedSessions/Source/AdvancedSessions/Classes/CreateSessionCallbackProxyAdvanced.h"
#include "Engine/World.h"

ULeageOfShooterGameInstance::ULeageOfShooterGameInstance()
{

}

TArray<FServerData>& ULeageOfShooterGameInstance::GetServerList()
{
	return ServerList;
}

void ULeageOfShooterGameInstance::GoToMap(FString MAPURL)
{
	GetWorld()->ServerTravel(MAPURL);
}
