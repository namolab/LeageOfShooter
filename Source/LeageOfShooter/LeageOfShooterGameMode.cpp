// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeageOfShooterGameMode.h"
#include "LeageOfShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALeageOfShooterGameMode::ALeageOfShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
