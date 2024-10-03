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
	UUI_Window* uiwindow = nullptr; //Forward Declaration pls ignore.

	///<summary>Event Delegate. Sets the currentUI to the defaultUI.</summary>
	UFUNCTION()
	void SetCurrentUIToDefault();

protected:
	///<summary>The currently focused UI_Window.</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information", meta=(Tooltip="The current UI in focus."))
	UUI_Window* currentUI;

	///<summary>A list of all UI_Windows this manager controls.</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information", meta = (Tooltip = "A list of all UI_Windows contained within this Manager."))
	TArray<UUI_Window*> UIList = {};

	///<summary>The default UI_Window.</summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Internal Information", meta = (Tooltip = "This Managers default UI_Window."))
	EUIID defaultUI = EUIID::NoID;

	///<summary>Sets currentUI to the UI_Window with the given UIID.</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI", meta = (Tooltip = "Sets the currentUI variable to a UI_Window that matches the given UUID. Also Swaps the Focus to the new UI."))
	void SetCurrentUIFromUIID(EUIID UIID);

	///<summary>Returns a UI_Window with the given UIID. Additionally, Returns false if the UI_Window does not exist.</summary>
	UFUNCTION(BlueprintPure, Category = "SDIO_UI", meta = (Tooltip = "Returns the UI_Window from UIList that matches the given UIID. Returns false if no such UI_Window exists."))
	bool GetUIFromUIID(EUIID UIID, UUI_Window*& window);

	///<summary>Initializes this object. THIS MUST BE RAN FROM BP OR IT WILL CRASH!</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI | Initialization", meta = (Tooltip = "Initializes the UI_Manager. This should go in the BP constructor."))
	void InitializeManager(UPARAM(ref) USDIO_UIWindow_Options*& optionsUI);

	///<summary>Navigates the UI system. Returns false if currentUI and defaultUI are the same.</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI | Initialization", meta = (Tooltip = "Enables this UI_Manager to handle UI Navigation. Should be connected to Enhanced User Input in BP. Returns False if the current UI and the default UI are the same. This should be setup to quit the application."))
	bool NavigateWindows();
};
