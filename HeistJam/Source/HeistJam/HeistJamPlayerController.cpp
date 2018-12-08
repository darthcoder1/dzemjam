// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HeistJamPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HeistJamCharacter.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogHeistController, Log, All);

AHeistJamPlayerController::AHeistJamPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	VelocityInput = FVector::ZeroVector;
	bIsFiring = false;
	bIsInteracting = false;
	Speed = 20.0f;
}

void AHeistJamPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	HandleInput(DeltaTime);
}

void AHeistJamPlayerController::HandleInput(float DeltaTime)
{
	VelocityInput = FVector::ZeroVector;
	VelocityInput.X = InputComponent->GetAxisValue("MoveX");
	VelocityInput.Y = InputComponent->GetAxisValue("MoveY");
	
	GetPawn()->AddMovementInput(VelocityInput);
}


void AHeistJamPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveX");
	InputComponent->BindAxis("MoveY");
	InputComponent->BindAction("Fire", IE_Pressed, this, &AHeistJamPlayerController::OnFirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AHeistJamPlayerController::OnFireReleased);
	InputComponent->BindAction("Interact", IE_Pressed, this, &AHeistJamPlayerController::OnInteractPressed);
	InputComponent->BindAction("Interact", IE_Released, this, &AHeistJamPlayerController::OnInteractReleased);
}

void AHeistJamPlayerController::OnFirePressed()
{
	bIsFiring = true;
}

void AHeistJamPlayerController::OnFireReleased ()
{
	bIsFiring = false;
}

void AHeistJamPlayerController::OnInteractPressed()
{
	bIsInteracting = true;
	OnInteractPressedCallback.Broadcast();
}

void AHeistJamPlayerController::OnInteractReleased()
{
	bIsInteracting = false;
	OnInteractReleasedCallback.Broadcast();
}