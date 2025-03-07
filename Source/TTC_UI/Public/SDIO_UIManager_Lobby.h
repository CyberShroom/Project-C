// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Manager.h"
#include "SDIO_UIManager_Lobby.generated.h"

/**
 * 
 */
UCLASS(meta = (ShortToolTip = "Lobby UI Manager base class."))
class TTC_UI_API USDIO_UIManager_Lobby : public UUI_Manager
{
	GENERATED_BODY()
	
protected:
	////Object References////
	UPROPERTY(BlueprintReadWrite, Category = "Hierarchy References", meta = (BindWidget))
	class UUI_Window* Lobby_Window;

	UPROPERTY(BlueprintReadWrite, Category = "Hierarchy References", meta = (BindWidget))
	class UUI_Window* Options_Window;

public:
	virtual void NativeConstruct() override;
};
