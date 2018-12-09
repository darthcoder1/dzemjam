// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HeistJamPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HeistJamCharacter.h"
#include "Engine/World.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "AIModule/Classes/AIController.h"
#include "UnrealNetwork.h"
#include "GameFramework/GameModeBase.h"
#include "HeistJamGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogHeistController, Log, All);

AHeistJamPlayerController::AHeistJamPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	VelocityInput = FVector::ZeroVector;
	bIsFiring = false;
	bIsInteracting = false;
	Speed = 20.0f;
	MaxFusionDistance = 100.0f;
	MaxFusionDuration = 10.0f;
	TimeSinceFusion = 0.0f;
}

void AHeistJamPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHeistJamPlayerController, InFusionWith);
	DOREPLIFETIME(AHeistJamPlayerController, FusionPawn);
	DOREPLIFETIME(AHeistJamPlayerController, TimerCountdown);
	DOREPLIFETIME(AHeistJamPlayerController, bIsTraitor);
}

bool AHeistJamPlayerController::CanInitiateFusion()
{
	return true;
}

void AHeistJamPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	HandleInput(DeltaTime);
	HandleFusion(DeltaTime);

	TimerCountdownString = FString::Printf(TEXT("%d:%2d"), (int)TimerCountdown / 60, (int)TimerCountdown % 60);
}

void AHeistJamPlayerController::HandleInput(float DeltaTime)
{
	if (!bIsInteracting && GetPawn())
	{
		VelocityInput = FVector::ZeroVector;
		VelocityInput.X = InputComponent->GetAxisValue("MoveX");
		VelocityInput.Y = InputComponent->GetAxisValue("MoveY");

		if (FusionPawn)
		{
			SERVER_AddFusionPawnMovement(VelocityInput * 0.5f);
		}
		else
		{
			GetPawn()->AddMovementInput(VelocityInput);
		}
	}
}

void AHeistJamPlayerController::HandleFusion(float DeltaTime)
{
	AHeistJamCharacter* me = GetCharacterPawn();
	if (!me)
		return;

	// This player is not in another fusion and both players have requested a Fusion
	if (me->RequestsFusionWith != NULL && me == me->RequestsFusionWith->RequestsFusionWith && InFusionWith == NULL)
	{
		UE_LOG(LogHeistController, Log, TEXT("Fusion for %s and %s initiated"), *me->GetName(), *me->RequestsFusionWith->GetName());

		InitiateFusion(me->RequestsFusionWith);
		SERVER_InitiateFusion(InFusionWith);
	}
	// A Fusion is active already, but one of the characters canceled the fusion request
	else if (InFusionWith && (me->RequestsFusionWith == NULL || InFusionWith->RequestsFusionWith != me))
	{
		AbortFusion();
		SERVER_AbortFusion();
		UE_LOG(LogHeistController, Log, TEXT("Fusion between %s and %s stopped"), *me->GetName(), *InFusionWith->GetName());
		InFusionWith = NULL;
	}
}

void AHeistJamPlayerController::InitiateFusion(AHeistJamCharacter* fusionTarget)
{
	GetPawn()->SetActorEnableCollision(false);
	GetPawn()->SetActorHiddenInGame(true);

	InFusionWith = fusionTarget;
	if (InFusionWith)
	{
		InFusionWith->SetActorEnableCollision(false);
		InFusionWith->SetActorHiddenInGame(true);
	}
}

void AHeistJamPlayerController::SERVER_InitiateFusion_Implementation(AHeistJamCharacter* fusionWith)
{
	static AHeistJamCharacter* s_stupidHack = NULL;
	
	InFusionWith = fusionWith;

	GetPawn()->SetActorEnableCollision(false);
	GetPawn()->SetActorHiddenInGame(true);
	
	if (s_stupidHack != NULL)
	{
		FusionPawn = s_stupidHack;
		s_stupidHack = NULL;
	}
	else
	{
		FVector loc = (GetPawn()->GetTransform().GetLocation() + InFusionWith->GetTransform().GetLocation()) / 2;
		FRotator rot = GetPawn()->GetTransform().GetRotation().Rotator();

		FActorSpawnParameters args;
		args.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		UClass* fusionPawnClass = GetFusionPawnClass(GetCharacterPawn()->CharacterType, InFusionWith->CharacterType);

		FusionPawn = GetWorld()->SpawnActor<AHeistJamCharacter>(fusionPawnClass, loc, rot, args);
		
		AAIController* ctrl = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());
		ctrl->Possess(FusionPawn);

		s_stupidHack = FusionPawn;
	}

	GetPawn()->AttachToActor(FusionPawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

bool AHeistJamPlayerController::SERVER_InitiateFusion_Validate(AHeistJamCharacter* fusionWith) { return true; }

void AHeistJamPlayerController::SERVER_AddFusionPawnMovement_Implementation(FVector vec)
{
	if (FusionPawn)
		FusionPawn->AddMovementInput(vec);
}

bool AHeistJamPlayerController::SERVER_AddFusionPawnMovement_Validate(FVector vec) { return true; }

void AHeistJamPlayerController::AbortFusion()
{
	if (FusionPawn)
		FusionPawn->SetActorEnableCollision(false);

	FVector offset = FVector::CrossProduct(GetPawn()->GetTransform().GetRotation().GetForwardVector().GetUnsafeNormal(), FVector::UpVector);

	GetPawn()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GetPawn()->SetActorLocation(GetPawn()->GetActorLocation() + offset * 10.0f + FVector::UpVector * 50.0f);
	GetPawn()->SetActorEnableCollision(true);
	GetPawn()->SetActorHiddenInGame(false);

	if (InFusionWith)
	{
		
		InFusionWith->SetActorLocation(InFusionWith->GetActorLocation() + offset * -10.0f + FVector::UpVector * 50.0f);
		InFusionWith->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		InFusionWith->SetActorEnableCollision(true);
		InFusionWith->SetActorHiddenInGame(false);
	}
}

void AHeistJamPlayerController::SERVER_AbortFusion_Implementation()
{
	FVector offset = FVector::CrossProduct(GetPawn()->GetTransform().GetRotation().GetForwardVector().GetUnsafeNormal(), FVector::UpVector);
	
	if (FusionPawn)
	{
		FusionPawn->SetActorEnableCollision(false);
	}

	
	GetPawn()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GetPawn()->SetActorLocation(GetPawn()->GetActorLocation() + offset * 10.0f + FVector::UpVector * 50.0f);
	GetPawn()->SetActorEnableCollision(true);
	GetPawn()->SetActorHiddenInGame(false);
	
	if (InFusionWith)
	{
		InFusionWith->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		InFusionWith->SetActorLocation(InFusionWith->GetActorLocation() + offset * -10.0f + FVector::UpVector * 50.0f);
		InFusionWith->SetActorEnableCollision(true);
		InFusionWith->SetActorHiddenInGame(false);
	}

	if (FusionPawn)
		FusionPawn->Destroy();

	FusionPawn = NULL;
}

bool AHeistJamPlayerController::SERVER_AbortFusion_Validate() { return true; }

void AHeistJamPlayerController::OnRep_IsTraitor()
{
	if (bIsTraitor)
	{
		OnIsTraitor.Broadcast();
	}

	SERVER_RestartMatch();
}

void AHeistJamPlayerController::RestartMatch()
{
	SERVER_RestartMatch_Implementation();
}

void AHeistJamPlayerController::SERVER_RestartMatch_Implementation()
{
	//GetWorld()->SeamlessTravel(TEXT("/TopDownCPP/Maps/TopDownExampleMap"));

	AHeistJamGameMode* gm = Cast<AHeistJamGameMode>(GetWorld()->GetAuthGameMode());

	if (gm)
		gm->ResetTimer();
}

bool AHeistJamPlayerController::SERVER_RestartMatch_Validate() { return true; }

AHeistJamCharacter * AHeistJamPlayerController::GetNearestOtherPawn(float maxDist)
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHeistJamCharacter::StaticClass(), actors);

	AHeistJamCharacter* nearestChar = NULL;
	float nearestCharDistSqr = FLT_MAX;

	const float maxDistSqr = maxDist * maxDist;

	FVector myPos = GetPawn()->GetTransform().GetLocation();

	for (int i = 0; i < actors.Num(); ++i)
	{
		if (actors[i] == GetPawn())
			continue;

		float sqrDist = FVector::DistSquared(actors[i]->GetTransform().GetLocation(), myPos);

		if (sqrDist < maxDistSqr && sqrDist < nearestCharDistSqr)
		{
			nearestChar = Cast<AHeistJamCharacter>(actors[i]);
			nearestCharDistSqr = sqrDist;
		}
	}

	return nearestChar;
}


UClass* AHeistJamPlayerController::GetFusionPawnClass(ECharacterClass charClass1, ECharacterClass charClass2)
{
	if (charClass1 == ECharacterClass::Bulky && charClass2 == ECharacterClass::Speedy ||
		charClass1 == ECharacterClass::Speedy && charClass2 == ECharacterClass::Bulky)
	{
		return FusionBulkySpeedyPawnClass;
	}
	else if (charClass1 == ECharacterClass::Bulky && charClass2 == ECharacterClass::Hacky ||
			 charClass1 == ECharacterClass::Hacky && charClass2 == ECharacterClass::Bulky)
	{
		return FusionBulkyHackyPawnClass;
	}
	else if (charClass1 == ECharacterClass::Speedy && charClass2 == ECharacterClass::Hacky ||
			 charClass1 == ECharacterClass::Hacky && charClass2 == ECharacterClass::Speedy)
	{
		return FusionSpeedyHackyPawnClass;
	}

	return NULL;
}


void AHeistJamPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveX");
	InputComponent->BindAxis("MoveY");
	InputComponent->BindAction("Fire", IE_Pressed, this, &AHeistJamPlayerController::OnFirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AHeistJamPlayerController::OnFireReleased);
	InputComponent->BindAction("QT_Face_Up", IE_Pressed, this, &AHeistJamPlayerController::OnUpPressed);
	InputComponent->BindAction("QT_Face_Down", IE_Pressed, this, &AHeistJamPlayerController::OnDownPressed);
	InputComponent->BindAction("QT_Face_Left", IE_Pressed, this, &AHeistJamPlayerController::OnLeftPressed);
	InputComponent->BindAction("QT_Face_Right", IE_Pressed, this, &AHeistJamPlayerController::OnRightPressed);
	InputComponent->BindAction("Interact", IE_Pressed, this, &AHeistJamPlayerController::OnInteractPressed);
	InputComponent->BindAction("Interact", IE_Released, this, &AHeistJamPlayerController::OnInteractReleased);

	InputComponent->BindAction("Fusion", IE_Pressed, this, &AHeistJamPlayerController::OnFusionPressed);
	InputComponent->BindAction("Fusion", IE_Released, this, &AHeistJamPlayerController::OnFusionReleased);
}

void AHeistJamPlayerController::OnFirePressed()
{
	if (!bIsInteracting && GetPawn())
	{
		bIsFiring = true;
		GetPawn()->PawnStartFire(1);
	}
}

void AHeistJamPlayerController::OnFireReleased()
{
	if (!bIsInteracting && GetPawn())
	{
		bIsFiring = false;
		GetPawn()->PawnStartFire(0);
	}
}

void AHeistJamPlayerController::OnInteractPressed()
{
	OnInteractPressedCallback.Broadcast();
}

void AHeistJamPlayerController::OnInteractReleased()
{
	OnInteractReleasedCallback.Broadcast();
}

void AHeistJamPlayerController::OnUpPressed() 
{
	OnQuickTimeEventPressedUp.Broadcast();
}
void AHeistJamPlayerController::OnDownPressed() 
{
	OnQuickTimeEventPressedDown.Broadcast();
}
void AHeistJamPlayerController::OnRightPressed() 
{
	OnQuickTimeEventPressedRight.Broadcast();
}
void AHeistJamPlayerController::OnLeftPressed() 
{
	OnQuickTimeEventPressedLeft.Broadcast();
}
void AHeistJamPlayerController::OnFusionPressed()
{
	AHeistJamCharacter* me = GetCharacterPawn();

	if (me)
	{
		AHeistJamCharacter* nearestNeighbor = GetNearestOtherPawn(MaxFusionDistance);
		me->SERVER_RequestFusionWith(nearestNeighbor);
		UE_LOG(LogHeistController, Log, TEXT("SERVER_RequestFusionWith(%s)"), nearestNeighbor ?
			*nearestNeighbor->GetName() : TEXT("NULL"));
	}
}

void AHeistJamPlayerController::OnFusionReleased()
{
	AHeistJamCharacter* me = GetCharacterPawn();

	if (me)
	{
		me->SERVER_RequestFusionWith(NULL);
		UE_LOG(LogHeistController, Log, TEXT("SERVER_RequestFusionWith(NULL)"));
	}
}