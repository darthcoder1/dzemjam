
#include "HeistWeapon.h"

AHeistWeapon::AHeistWeapon()
{

}


void AHeistWeapon::StartFire(uint8 modeNum)
{
	BP_StartFire(modeNum);
}