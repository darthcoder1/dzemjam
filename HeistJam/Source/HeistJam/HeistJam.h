// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHeistJam, Log, All);

UENUM(BlueprintType)
enum class ECharacterClass : uint8 
{
	None,
	Bulky,
	Speedy,
	Hacky
};