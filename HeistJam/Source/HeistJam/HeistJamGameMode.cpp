// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HeistJamGameMode.h"
#include "HeistJamPlayerController.h"
#include "HeistJamCharacter.h"
#include "HeistPlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"

AHeistJamGameMode::AHeistJamGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AHeistJamPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/Bulky"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> SpeedyPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/Speedy"));
	if (SpeedyPawnBPClass.Class != NULL)
	{
		SpeedyPawnClass = SpeedyPawnBPClass.Class;
	}

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> BulkyPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/Bulky"));
	if (BulkyPawnBPClass.Class != NULL)
	{
		BulkyPawnClass = BulkyPawnBPClass.Class;
	}

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> HackyPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/Hacky"));
	if (HackyPawnBPClass.Class != NULL)
	{
		HackyPawnClass = HackyPawnBPClass.Class;
	}

	AlarmTriggered = false;
	TimeSinceMatchStart = 0.0f;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AHeistJamGameMode::RestartPlayerAtPlayerStart(AController * NewPlayer, AActor * StartSpot)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (!StartSpot)
	{
		UE_LOG(LogGameMode, Warning, TEXT("RestartPlayerAtPlayerStart: Player start not found"));
		return;
	}

	FRotator SpawnRotation = StartSpot->GetActorRotation();

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart %s"), (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(NewPlayer)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart: Tried to restart a spectator-only player!"));
		return;
	}

	if (NewPlayer->GetPawn() != nullptr)
	{
		// If we have an existing pawn, just use it's rotation
		SpawnRotation = NewPlayer->GetPawn()->GetActorRotation();
	}
	else
	{
		// Don't allow pawn to be spawned with any pitch or roll
		FRotator StartRotation(ForceInit);
		StartRotation.Yaw = StartSpot->GetActorRotation().Yaw;
		FVector StartLocation = StartSpot->GetActorLocation();

		FTransform Transform = FTransform(StartRotation, StartLocation);

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map

		UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);

		AHeistPlayerStart* playerStart = Cast<AHeistPlayerStart>(StartSpot);
		if (playerStart)
		{
			switch (playerStart->SpawnCharacterType)
			{
				case ECharacterClass::Speedy:
					PawnClass = SpeedyPawnClass;
					break;
				case ECharacterClass::Bulky:
					PawnClass = BulkyPawnClass;
					break;
				case ECharacterClass::Hacky:
					PawnClass = HackyPawnClass;
					break;
			}
		}
		
		APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, Transform, SpawnInfo);
		if (!ResultPawn)
		{
			UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *Transform.ToHumanReadableString());
		}

		// Try to create a pawn to use of the default class for this player
		NewPlayer->SetPawn(ResultPawn);
	}

	if (NewPlayer->GetPawn() == nullptr)
	{
		NewPlayer->FailedToSpawnPawn();
	}
	else
	{
		// Tell the start spot it was used
		InitStartSpot(StartSpot, NewPlayer);

		FinishRestartPlayer(NewPlayer, SpawnRotation);
	}
}

void AHeistJamGameMode::BeginPlay()
{
	Super::BeginPlay();

	TimeSinceMatchStart = 0.0f;
	AlarmTriggered = false;
}

void AHeistJamGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeSinceMatchStart += DeltaSeconds;

	if (TimeSinceMatchStart > InfiltrationTime && !AlarmTriggered)
	{
		AlarmTriggered = true;
	}
	else if (TimeSinceMatchStart > InfiltrationTime + EscapeTime)
	{
		//
	}


	float timeLeft = InfiltrationTime + EscapeTime - TimeSinceMatchStart;

	// start human players first
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			AHeistJamPlayerController* pc = Cast<AHeistJamPlayerController>(PlayerController);
			pc->TimerCountdown = timeLeft;
		}
	}
}
