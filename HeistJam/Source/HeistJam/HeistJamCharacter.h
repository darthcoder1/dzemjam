// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HeistWeapon.h"
#include "HeistJam.h"
#include "GameFramework/Character.h"
#include "HeistJamCharacter.generated.h"

UCLASS(Blueprintable)
class AHeistJamCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHeistJamCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AHeistWeapon> WeaponClass;

	UPROPERTY(Transient)
	AHeistWeapon* WeaponInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ECharacterClass CharacterType;

	UPROPERTY(BlueprintReadWrite)
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth;
	
	virtual void BeginPlay() override;

	virtual void PawnStartFire(uint8 FireModeNum) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

};

