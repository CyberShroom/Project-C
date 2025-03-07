// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Manager.h"
#include "Item_Slot.h"
#include "SDIO_UIManager_Game.generated.h"

/**
 * 
 */
UCLASS(meta = (ShortToolTip = "Game UI Manager base class."))
class TTC_UI_API USDIO_UIManager_Game : public UUI_Manager
{
	GENERATED_BODY()



protected:
	////Object References////
	UPROPERTY(BlueprintReadWrite, Category = "Hierarchy References", meta = (BindWidget))
	class UUI_Window* Options_Window;

public:
	////Object References////
	UPROPERTY(BlueprintReadWrite, Category = "Hierarchy References", meta = (BindWidget))
	class UUI_Window* Inventory_Window;

	virtual void NativeConstruct() override;
};
