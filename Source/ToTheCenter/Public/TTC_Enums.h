#pragma once

#include "TTC_Enums.generated.h"

//A list of enums that refer to the different health pools in the game.
UENUM(BlueprintType, Category = "TTC | Enums", meta = (Tooltip = "A list of enums that refer to the different health pools in the game."))
enum class EHealthPools : uint8
{
	HULL,
	ARMOR,
	SHIELD
};
