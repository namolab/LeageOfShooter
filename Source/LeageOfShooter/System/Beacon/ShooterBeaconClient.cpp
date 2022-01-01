// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBeaconClient.h"

AShooterBeaconClient::AShooterBeaconClient()
{

}
//
//void AShooterBeaconClient::OnFailure()
//{
//	FOnConnected.Broadcast(false);
//}
//
//void AShooterBeaconClient::OnConnected()
//{
//	UE_LOG(LogTemp, Warning, TEXT("SuccessClient"));
//	FOnConnected.Broadcast(true);
//}
//
//void AShooterBeaconClient::SendChatMessage(const FText& ChatMessage)
//{
//	Server_SendChatMessage(ChatMessage);
//}
//
//bool AShooterBeaconClient::Server_SendChatMessage_Validate(const FText& ChatMessage)
//{
//	return true;
//}
//
//void AShooterBeaconClient::Server_SendChatMessage_Implementation(const FText& ChatMessage)
//{
//	FString Message = PlayerName + ": " + ChatMessage.ToString();
//	UE_LOG(LogTemp, Warning, TEXT("Chat : %s"), *Message);
//	if (ANaziZombieBeaconHostObject* Host = Cast<ANaziZombieBeaconHostObject>(BeaconOwner))
//	{
//		Host->SendChatToLobby(FText::FromString(Message));
//	}
//}
//
//void AShooterBeaconClient::Client_OnChatMessageReceived_Implementation(const FText& ChatMessage)
//{
//	FOnChatReceived.Broadcast(ChatMessage);
//}
//
//void AShooterBeaconClient::Client_OnDisconnected_Implementation()
//{
//	FOnDisconnected.Broadcast();
//}
//
//void AShooterBeaconClient::Client_OnLobbyUpdated_Implementation(FNaziZombieLobbyInfo LobbyInfo)
//{
//	FOnLobbyUpdated.Broadcast(LobbyInfo);
//}
//
//void AShooterBeaconClient::Client_FullConnect_Implementation()
//{
//	FOnFullConnect.Broadcast();
//}
//
//void AShooterBeaconClient::SetPlayerIndex(uint8 index)
//{
//	PlayerIndex = index;
//}
//
//uint8 AShooterBeaconClient::GetPlayerIndex()
//{
//	return PlayerIndex;
//}
//
//void AShooterBeaconClient::SetPlayerName(const FString& NewPlayerName)
//{
//	PlayerName = NewPlayerName;
//}
//
//FString AShooterBeaconClient::GetPlayerName()
//{
//	return PlayerName;
//}
//
//bool AShooterBeaconClient::ConnectToServer(const FString& Address)
//{
//	FURL Destination = FURL(nullptr, *Address, ETravelType::TRAVEL_Absolute);
//	Destination.Port = 7787;
//	return InitClient(Destination);
//}
//
//void AShooterBeaconClient::FullConnectToServer(const FString& JoinAddress)
//{
//	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Success"));
//		PC->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
//	}
//
//	LeaveLobby();
//}
//
//void AShooterBeaconClient::LeaveLobby()
//{
//	UE_LOG(LogTemp, Warning, TEXT("Client Beacon Corrupt"));
//	DestroyBeacon();
//
//}