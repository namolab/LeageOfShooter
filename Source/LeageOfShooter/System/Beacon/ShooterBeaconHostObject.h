// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "Http.h"
#include "LeageOfShooter/System/Instance/LeageOfShooterGameInstance.h"
#include "ShooterBeaconHostObject.generated.h"


UCLASS()
class LEAGEOFSHOOTER_API AShooterBeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()
	
public:
	AShooterBeaconHostObject();


protected:
//	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
//
//	UFUNCTION(BlueprintCallable)
//	void SetServerData(FServerData NewServerData);
//
//	UFUNCTION(BlueprintCallable)
//	void UpdateServerData(FServerData NewServerData);
//
//	UFUNCTION(BlueprintCallable)
//	int GetCurrentPlayerCount();
//
//
//	UPROPERTY(BlueprintAssignable)
//	FHostLobbyUpdated FOnHostLobbyUpdated;
//
//	UPROPERTY(BlueprintAssignable)
//	FHostChatReceived FOnHostChatReceived;
//
//	FTimerHandle TInitialLobbyHandle;
//	void InitialLobbyHandling();
//
//	UFUNCTION(BlueprintCallable)
//	void UpdateLobbyInfo(FNaziZombieLobbyInfo NewLobbyInfo);
//
//	void UpdateClientLobbyInfo();
//
//	virtual void BeginPlay() override;
//
	//virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;
//
//	virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor) override;
//
//	UFUNCTION(BlueprintCallable)
//	void ShutdownServer();
//
//	void DisconnectAllClients();
//
//	virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;
//
//	UFUNCTION(BlueprintCallable)
//	void StartServer(const FString& MapURL);
//
//public:
//	UFUNCTION(BlueprintCallable)
//	void SendChatToLobby(const FText& ChatMessage);

//
//protected:
//	FHttpModule* Http;
//	int ServerID;
	//FServerData ServerData;
	//FNaziZombieLobbyInfo LobbyInfo;
};
