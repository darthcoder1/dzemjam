#pragma once

#include "ProjectileBase.h"
#include "HeistWeapon.generated.h"


UCLASS()
class AHeistWeapon : public AActor
{
	GENERATED_BODY()

public:
	AHeistWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float RefireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;

	float timeSinceLastShot;

	bool bIsFiring;


	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartFire(uint8 modeNum);


	void StartFire(uint8 modeNum);

	virtual void Tick(float DeltaSeconds);
};