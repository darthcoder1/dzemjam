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
	UPROPERTY(EditDefaultsOnly)
	float InfiltrationTime;

	UPROPERTY(EditDefaultsOnly)
	float EscapeTime;

	UPROPERTY(BlueprintReadWrite)
	float TimeSinceMatchStart;

	UPROPERTY(BlueprintReadWrite)
	bool AlarmTriggered;

	UPROPERTY(Transient)
	class AHeistJamPlayerController* TraitorPC;

	AHeistJamGameMode();

	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


	class AHeistJamPlayerController* PickATraitor();
};



