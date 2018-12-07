// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HeistJamGameMode.h"
#include "HeistJamPlayerController.h"
#include "HeistJamCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHeistJamGameMode::AHeistJamGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AHeistJamPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}