// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeistJamPlayerController.generated.h"

UCLASS()
class AHeistJamPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHeistJamPlayerController();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickTimeEventPressedSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractSignature);
protected:

	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteractPressedCallback;
	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteractReleasedCallback;

	UPROPERTY(BlueprintAssignable)
		FOnQuickTimeEventPressedSignature OnQuickTimeEventPressedUp;
	UPROPERTY(BlueprintAssignable)
		FOnQuickTimeEventPressedSignature OnQuickTimeEventPressedDown;
	UPROPERTY(BlueprintAssignable)
		FOnQuickTimeEventPressedSignature OnQuickTimeEventPressedRight;
	UPROPERTY(BlueprintAssignable)
		FOnQuickTimeEventPressedSignature OnQuickTimeEventPressedLeft;
	float Speed;
	FVector VelocityInput;
	
	uint32 bIsFiring : 1;

	UPROPERTY(BlueprintReadWrite)
	bool bIsInteracting = false;

	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	// Process input, apply movement, handle interactions
	void HandleInput(float DeltaTime);


	/** Input handlers  */
	void OnMoveX();
	void OnMoveY();
	void OnFirePressed();
	void OnFireReleased();
	void OnInteractPressed();
	void OnInteractReleased();
	void OnUpPressed();
	void OnDownPressed();
	void OnRightPressed();
	void OnLeftPressed();
};


