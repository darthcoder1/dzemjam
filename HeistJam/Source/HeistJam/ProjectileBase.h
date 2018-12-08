#pragma once

#include "Engine/Classes/Gameframework/DamageType.h"
#include "ProjectileBase.generated.h"

UCLASS(MinimalAPI)
class AProjectileBase : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLifetimePassedSignature);

public:
	AProjectileBase();

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UDamageType> DamageType;
};