// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "LeageOfShooterGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapURL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapDescription;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* MapImage;
};

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ServerID;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString IPAddress;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ServerName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentPlayers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxPlayers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Method;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Mode;

};

UCLASS()
class LEAGEOFSHOOTER_API ULeageOfShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	ULeageOfShooterGameInstance();

	UFUNCTION(BlueprintCallable)
	TArray<FServerData>& GetServerList();

	UFUNCTION(BlueprintCallable)
	void GoToMap(FString MAPURL);

	//static void CreateAdvanceSession();
public:
	TArray<FServerData> ServerList;

};
