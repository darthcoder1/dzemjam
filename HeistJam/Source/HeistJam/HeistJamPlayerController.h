// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeistJamCharacter.h"
#include "HeistJamPlayerController.generated.h"

UCLASS()
class AHeistJamPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHeistJamPlayerController();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickTimeEventPressedSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractSignature);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIsTraitorSignature);

	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteractPressedCallback;
	UPROPERTY(BlueprintAssignable)
	FOnInteractSignature OnInteractReleasedCallback;

	UPROPERTY(BlueprintAssignable)
	FOnIsTraitorSignature OnIsTraitor;

	UPROPERTY(BlueprintAssignable)
	FOnQuickTimeEventPressedSignature OnQuickTimeEventPressedUp;
	UPROPERTY(BlueprintAssignable)
	FOnQuickTimeEventPressedSignature OnQuickTimeEventPressedDown;
	UPROPERTY(BlueprintAssignable)
	FOnQuickTimeEventPressedSignature OnQuickTimeEventPressedRight;
	UPROPERTY(BlueprintAssignable)
	FOnQuickTimeEventPressedSignature OnQuickTimeEventPressedLeft;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> FusionBulkySpeedyPawnClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> FusionBulkyHackyPawnClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> FusionSpeedyHackyPawnClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxFusionDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FusionCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxFusionDistance;

	UPROPERTY(Replicated, BlueprintReadWrite, ReplicatedUsing=OnRep_IsTraitor)
	bool bIsTraitor;
	

	UPROPERTY(Replicated)
	AHeistJamCharacter* FusionPawn;
	
	UPROPERTY(Replicated, BlueprintReadWrite)
	float TimerCountdown;

	UPROPERTY(BlueprintReadOnly)
	FString TimerCountdownString;

	UPROPERTY(Transient)
	AHeistJamCharacter* OriginalPawnWhenFusion ;

	float TimeSinceFusion;

	float Speed;
	FVector VelocityInput;
	
	uint32 bIsFiring : 1;

	UPROPERTY(BlueprintReadWrite)
	bool bIsInteracting = false;

	UPROPERTY(Replicated)
	AHeistJamCharacter* InFusionWith;

	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	AHeistJamCharacter* GetCharacterPawn() { return Cast<AHeistJamCharacter>(GetPawn()); }

	// Process input, apply movement, handle interactions
	void HandleInput(float DeltaTime);
	void HandleFusion(float DeltaTime);

	void InitiateFusion(AHeistJamCharacter* fusionTarget);
	void AbortFusion();

	UFUNCTION(Server, Reliable, WithValidation)
	void SERVER_InitiateFusion(AHeistJamCharacter* fusionWith);

	UFUNCTION(Server, Reliable, WithValidation)
	void SERVER_AddFusionPawnMovement(FVector vec);

	UFUNCTION(Server, Reliable, WithValidation)
	void SERVER_AbortFusion();

	UFUNCTION()
	void OnRep_IsTraitor();


	AHeistJamCharacter* GetNearestOtherPawn(float maxDist);

	UClass* GetFusionPawnClass(ECharacterClass charClass1, ECharacterClass charClass2);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	bool CanInitiateFusion();

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
	void OnFusionPressed();
	void OnFusionReleased();
};


