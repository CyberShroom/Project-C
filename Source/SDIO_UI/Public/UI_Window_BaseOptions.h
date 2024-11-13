#pragma once

#include "CoreMinimal.h"
#include "UI_Window.h"
#include "UI_Window_BaseOptions.generated.h"

UCLASS(meta = (ShortToolTip = "UI_Window derived class for the options UI, since its used in every level."))
class SDIO_UI_API UUI_Window_BaseOptions : public UUI_Window
{
	GENERATED_BODY()

public:
	/// <summary>Reference to the UI_Managers defaultUI</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	EUIID defaultUI = EUIID::NoID;
};

