// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HeistJamGameMode.generated.h"

UCLASS(minimalapi)
class AHeistJamGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> SpeedyPawnClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> BulkyPawnClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> HackyPawnClass;

public:
	AHeistJamGameMode();

	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
};



