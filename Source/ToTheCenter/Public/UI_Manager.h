// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Master.h"
#include "SDIO_UI_Enums.h"
#include "SDIO_UIWindow_Options.h"
#include "UI_Manager.generated.h"

/**
 * 
 */

class UUI_Window; //Forward Declaration

UCLASS(meta = (ShortToolTip = "This should be the parent of all UI_Windows. Every level must have only 1. This controls the overall SDIO_UI logic."))
class TOTHECENTER_API UUI_Manager : public UUI_Master
{
	GENERATED_BODY()

private:
	UUI_Window* uiwindow = nullptr;

	UFUNCTION()
	void SetCurrentUI(EUIID UIID);
	UFUNCTION()
	void SetCurrentUIToDefault();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Internal Information", meta=(Tooltip="The current UI in focus."))
	UUI_Window* currentUI;
	UPROPERTY(BlueprintReadWrite, Category = "Internal Information", meta = (Tooltip = "A list of all UI_Windows contained within this Manager."))
	TArray<UUI_Window*> UIList = {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="INSTANCE DEPENDANT", meta = (Tooltip = "This Managers default UI_Window."))
	EUIID defaultUI = EUIID::NoID;

	UFUNCTION(BlueprintCallable, Category = "SDIO_UI", meta = (Tooltip = "Sets the currentUI variable to a UI_Window that matches the given UUID. Also Swaps the Focus to the new UI."))
	void SetCurrentUIFromUIID(EUIID UIID);

	UFUNCTION(BlueprintPure, Category = "SDIO_UI", meta = (Tooltip = "Returns the UI_Window from UIList that matches the given UIID. Returns false if no such UI_Window exists."))
	bool GetUIFromUIID(EUIID UIID, UUI_Window*& window);

	UFUNCTION(BlueprintCallable, Category = "SDIO_UI | Initialization", meta = (Tooltip = "Initializes the UI_Manager. This should go in the BP constructor."))
	void InitializeManager(UPARAM(ref) USDIO_UIWindow_Options*& optionsUI);
};
