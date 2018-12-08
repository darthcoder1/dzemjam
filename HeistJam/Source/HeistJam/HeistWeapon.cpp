
#include "HeistWeapon.h"
#include "HeistJamCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHeistWeapon::AHeistWeapon()
{
	DamageTypeClass = UDamageType::StaticClass();
	bIsFiring = false;
	
	SetActorTickEnabled(true);
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.SetTickFunctionEnable(true);
	PrimaryActorTick.bStartWithTickEnabled = true;
}


void AHeistWeapon::StartFire(uint8 modeNum)
{
	bIsFiring = modeNum == 1;
	timeSinceLastShot = FLT_MAX;
	BP_StartFire(modeNum);
} 

void AHeistWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	timeSinceLastShot += DeltaSeconds;

	if (bIsFiring)
	{
		if (timeSinceLastShot > RefireRate)
		{
			AHeistJamCharacter* pawn = Cast<AHeistJamCharacter>(GetOwner());

			if (pawn)
			{
				FVector loc = GetTransform().GetLocation();
				FRotator rot = GetTransform().GetRotation().Rotator();

				FActorSpawnParameters args;
				args.Owner = this;

				AProjectileBase* proj = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, loc, rot, args);
			}

			timeSinceLastShot = 0.0f;
		}
	}
}