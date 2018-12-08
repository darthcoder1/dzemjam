#pragma once

#include "HeistWeapon.generated.h"

UCLASS()
class AHeistWeapon : public AActor
{
	GENERATED_BODY()

public:
	AHeistWeapon();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartFire(uint8 modeNum);


	void StartFire(uint8 modeNum);
};