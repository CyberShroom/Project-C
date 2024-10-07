// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Manager.h"
#include "SDIO_UIManager_Game.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API USDIO_UIManager_Game : public UUI_Manager
{
	GENERATED_BODY()
	
protected:
	////Object References////
	UPROPERTY(BlueprintReadWrite, Category = "Hierarchy References", meta = (BindWidget))
	class UUI_Window* Inventory_Window;

	UPROPERTY(BlueprintReadWrite, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UIWindow_Options* Options_Window;

public:
	virtual void NativeConstruct() override;
};
