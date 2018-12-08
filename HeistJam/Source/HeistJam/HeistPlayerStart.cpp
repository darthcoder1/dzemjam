#include "HeistPlayerStart.h"

AHeistPlayerStart::AHeistPlayerStart(const FObjectInitializer& ObjectInitializer)
	:APlayerStart(ObjectInitializer)
{
	SpawnCharacterType = ECharacterClass::None;
}