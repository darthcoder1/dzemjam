#pragma once

#include "GameFramework/PlayerStart.h"
#include "HeistJam.h"
#include "HeistPlayerStart.generated.h"


UCLASS()
class AHeistPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	AHeistPlayerStart(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	ECharacterClass SpawnCharacterType;
};