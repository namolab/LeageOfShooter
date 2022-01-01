// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "LeageOfShooter/System/Beacon/ShooterBeaconHostObject.h"
#include "ShooterBeaconClient.generated.h"


UCLASS()
class LEAGEOFSHOOTER_API AShooterBeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()
	
public:
	AShooterBeaconClient();

//protected:
//	UFUNCTION(BlueprintCallable)
//	bool ConnectToServer(const FString& Address);

	//UFUNCTION(BlueprintCallable)
	//void FullConnectToServer(const FString& JoinAddress);

	//UFUNCTION(BlueprintCallable)
	//void LeaveLobby();

	//virtual void OnFailure() override;
	//virtual void OnConnected() override;

//protected:
//	UFUNCTION(BlueprintCallable)
//	void SendChatMessage(const FText& ChatMessage);
//
//	UFUNCTION(Server, Reliable)
//	void CS_SendChatMessage(const FText& ChatMessage);
//
//	UFUNCTION(Client, Reliable)
//		void SC_Disconnected();
//
//	UFUNCTION(Client, Reliable)
//		void SC_LobbyUpdated(FNaziZombieLobbyInfo LobbyInfo);
//
//	UFUNCTION(Client, Reliable)
//		void SC_ChatMessageReceived(const FText& ChatMessage);
//	void SC_ChatMessageReceived_Implementation(const FText& ChatMessage);
//
//	UFUNCTION(Client, Reliable)
//		void SC_FullConnect();
//
//public:
//	void SetPlayerIndex(uint8 index);
//	uint8 GetPlayerIndex();
//
//	void SetPlayerName(const FString& NewPlayerName);
//	FString GetPlayerName();
//
//
//protected:
//	UPROPERTY(BlueprintAssignable)
//	FConnectSuccess FOnConnected;
//
//	UPROPERTY(BlueprintAssignable)
//	FDisconnected FOnDisconnected;
//
//	UPROPERTY(BlueprintAssignable)
//	FLobbyUpdated FOnLobbyUpdated;
//
//	UPROPERTY(BlueprintAssignable)
//	FChatReceived FOnChatReceived;
//
//	UPROPERTY(BlueprintAssignable)
//	FFullConnect FOnFullConnect;
//
//	uint8 PlayerIndex;
//	FString PlayerName;
};
