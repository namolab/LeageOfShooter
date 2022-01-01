// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterBeaconHostObject.h"
#include "ShooterBeaconClient.h"
#include "LeageOfShooter/System/Instance/LeageOfShooterGameInstance.h"
#include "OnlineBeaconHost.h"
#include "TimerManager.h"

AShooterBeaconHostObject::AShooterBeaconHostObject()
{
	//ClientBeaconActorClass = AShooterBeaconClient::StaticClass();
	//BeaconTypeName = ClientBeaconActorClass->GetName();

	//Http = &FHttpModule::Get();
	//ServerID = -1;
}
//
//void AShooterBeaconHostObject::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
//{
//	if (Success)
//	{
//		ServerID = FCString::Atoi(*Response->GetContentAsString());
//	}
//	else
//	{
//
//	}
//}
//
//void AShooterBeaconHostObject::SetServerData(FServerData NewServerData)
//{
//	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
//
//	ServerData = NewServerData;
//	ServerData.CurrentPlayers = GetCurrentPlayerCount();
//
//
//	JsonObject->SetNumberField("ServerID", 0);
//	JsonObject->SetStringField("IPAddress", "127.0.0.1");
//	JsonObject->SetStringField("ServerName", ServerData.ServerName);
//	JsonObject->SetStringField("MapName", ServerData.MapName);
//	JsonObject->SetNumberField("CurrentPlayers", ServerData.CurrentPlayers);
//	JsonObject->SetNumberField("MaxPlayers", ServerData.MaxPlayers);
//
//	FString JsonString;
//	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
//	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
//
//	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
//
//	Request->OnProcessRequestComplete().BindUObject(this, &ANaziZombieBeaconHostObject::OnProcessRequestComplete);
//
//	Request->SetURL("https://localhost:44392/api/Host");
//	Request->SetVerb("POST");
//	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
//	Request->SetContentAsString(JsonString);
//	Request->ProcessRequest();
//}
//
//void AShooterBeaconHostObject::UpdateServerData(FServerData NewServerData)
//{
//	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
//	ServerData = NewServerData;
//	ServerData.CurrentPlayers = GetCurrentPlayerCount();
//
//	JsonObject->SetNumberField("ServerID", 0);
//	JsonObject->SetStringField("IPAddress", "127.0.0.1");
//	JsonObject->SetStringField("ServerName", ServerData.ServerName);
//	JsonObject->SetStringField("MapName", ServerData.MapName);
//	JsonObject->SetNumberField("CurrentPlayers", ServerData.CurrentPlayers);
//	JsonObject->SetNumberField("MaxPlayers", ServerData.MaxPlayers);
//
//	FString JsonString;
//	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
//	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
//
//	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
//
//	Request->OnProcessRequestComplete().BindUObject(this, &ANaziZombieBeaconHostObject::OnProcessRequestComplete);
//
//	Request->SetURL("https://localhost:44392/api/Host/1");
//	Request->SetVerb("PUT");
//	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
//	Request->SetContentAsString(JsonString);
//	Request->ProcessRequest();
//}
//
//int AShooterBeaconHostObject::GetCurrentPlayerCount()
//{
//	return LobbyInfo.PlayerList.Num();
//}
//
//void AShooterBeaconHostObject::InitialLobbyHandling()
//{
//	UpdateLobbyInfo(LobbyInfo);
//
//}
//
//void AShooterBeaconHostObject::UpdateLobbyInfo(FNaziZombieLobbyInfo NewLobbyInfo)
//{
//	LobbyInfo.MapImage = NewLobbyInfo.MapImage;
//	UpdateClientLobbyInfo();
//	FOnHostLobbyUpdated.Broadcast(LobbyInfo);
//}
//
//void AShooterBeaconHostObject::UpdateClientLobbyInfo()
//{
//	for (AOnlineBeaconClient* ClientBeacon : ClientActors)
//	{
//		if (ANaziZombieBeaconClient* Client = Cast<ANaziZombieBeaconClient>(ClientBeacon))
//		{
//			Client->Client_OnLobbyUpdated(LobbyInfo);
//		}
//	}
//}
//
//void AShooterBeaconHostObject::BeginPlay()
//{
//	LobbyInfo.PlayerList.Add(FString("Host"));
//	GetWorld()->GetTimerManager().SetTimer(TInitialLobbyHandle, this, &ANaziZombieBeaconHostObject::InitialLobbyHandling, 0.2f, false);
//}
//
//void AShooterBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
//{
//	Super::OnClientConnected(NewClientActor, ClientConnection);
//
//	if (NewClientActor)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("SuccessHost"));
	/*	if (GetCurrentPlayerCount() >= ServerData.MaxPlayers)
		{
			if (ANaziZombieBeaconClient* Client = Cast<ANaziZombieBeaconClient>(NewClientActor))
			{
				Client->SetPlayerIndex(240);
				DisconnectClient(NewClientActor);
				return;
			}

		}

		FString PlayerName = FString("Player");
		uint8 Index = LobbyInfo.PlayerList.Num();
		PlayerName.Append(FString::FromInt(Index));
		LobbyInfo.PlayerList.Add(PlayerName);

		if (ANaziZombieBeaconClient* Client = Cast<ANaziZombieBeaconClient>(NewClientActor))
		{
			Client->SetPlayerIndex(Index);
			Client->SetPlayerName(PlayerName);
		}
		FOnHostLobbyUpdated.Broadcast(LobbyInfo);
		UpdateClientLobbyInfo();
		UpdateServerData(ServerData);*/
//	}
//}
//
//void AShooterBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
//{
//	Super::NotifyClientDisconnected(LeavingClientActor);
//
//	if (ANaziZombieBeaconClient* Client = Cast<ANaziZombieBeaconClient>(LeavingClientActor))
//	{
//		uint8 Index = Client->GetPlayerIndex();
//		if (Index == 240)
//		{
//			return;
//		}
//		LobbyInfo.PlayerList.RemoveAt(Index);
//	}
//
//	FOnHostLobbyUpdated.Broadcast(LobbyInfo);
//	UpdateClientLobbyInfo();
//	UpdateServerData(ServerData);
//}
//
//void AShooterBeaconHostObject::ShutdownServer()
//{
//	DisconnectAllClients();
//
//	if (AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
//	{
//		//	Host->UnregisterHost(BeaconTypeName);
//		Host->DestroyBeacon();
//	}
//	if (ServerID != -1)
//	{
//		TSharedRef<IHttpRequest> Request = Http->CreateRequest();
//
//		Request->SetURL("https://localhost:44392/api/Host/" + FString::FromInt(ServerID));
//		Request->SetVerb("DELETE");
//		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
//
//		Request->ProcessRequest();
//	}
//}
//
//void AShooterBeaconHostObject::DisconnectAllClients()
//{
//	for (AOnlineBeaconClient* Client : ClientActors)
//	{
//		if (Client)
//		{
//			DisconnectClient(Client);
//		}
//	}
//}
//
//void AShooterBeaconHostObject::DisconnectClient(AOnlineBeaconClient* ClientActor)
//{
//	AOnlineBeaconHost* BeaconHost = Cast<AOnlineBeaconHost>(GetOwner());
//	if (BeaconHost)
//	{
//		if (ANaziZombieBeaconClient* Client = Cast<ANaziZombieBeaconClient>(ClientActor))
//		{
//			Client->Client_OnDisconnected();
//		}
//		UE_LOG(LogTemp, Warning, TEXT("Host-Client Disconnect"));
//		BeaconHost->DisconnectClient(ClientActor);
//	}
//}
//
//void AShooterBeaconHostObject::StartServer(const FString& MapURL)
//{
//	for (AOnlineBeaconClient* ClientBeacon : ClientActors)
//	{
//		if (ANaziZombieBeaconClient* Client = Cast<ANaziZombieBeaconClient>(ClientBeacon))
//		{
//			Client->Client_FullConnect();
//		}
//	}
//	GetWorld()->ServerTravel(MapURL + "?listen");
//	ShutdownServer();
//}
//
//void AShooterBeaconHostObject::SendChatToLobby(const FText& ChatMessage)
//{
//	FOnHostChatReceived.Broadcast(ChatMessage);
//	for (AOnlineBeaconClient* ClientBeacon : ClientActors)
//	{
//		if (ANaziZombieBeaconClient* Client = Cast<ANaziZombieBeaconClient>(ClientBeacon))
//		{
//			Client->Client_OnChatMessageReceived(ChatMessage);
//		}
//	}
//}
