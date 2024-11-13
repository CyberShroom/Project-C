// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Master.h"
#include "SDIO_UI_Enums.h"
#include "UI_Window_BaseOptions.h"
#include "UI_Manager.generated.h"

/**
 * 
 */

class UUI_Window; //Forward Declaration

UCLASS(meta = (ShortToolTip = "Contains all UI_Windows that are present in the level. Every level must have one and only 1. Controls overall SDIO_UI logic."))
class SDIO_UI_API UUI_Manager : public UUI_Master
{
	GENERATED_BODY()

private:
	UUI_Window* uiwindow = nullptr; //Forward Declaration pls ignore.

protected:
	///<summary>The currently focused UI_Window.</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta=(Tooltip="The current UI in focus."))
	UUI_Window* currentUI;

	///<summary>A list of all UI_Windows this manager controls.</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "A list of all UI_Windows contained within this Manager."))
	TArray<UUI_Window*> UIList = {};

	///<summary>The default UI_Window.</summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config", meta = (Tooltip = "This Managers default UI_Window."))
	EUIID defaultUI = EUIID::NoID;

	///<summary>Returns a UI_Window with the given UIID. Additionally, Returns false if the UI_Window does not exist.</summary>
	UFUNCTION(BlueprintPure, Category = "SDIO_UI", meta = (Tooltip = "Returns the UI_Window from UIList that matches the given UIID. If no UI with a matching UIID exists, return the current UI.\n Returns true if a UI is found. Returns false is no UI is found."))
	bool GetUIFromUIID(EUIID UIID, UUI_Window*& window);

	///<summary>Initializes this object. THIS MUST BE RAN FROM BP OR IT WILL CRASH!</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI | Initialization", meta = (Tooltip = "Initializes the UI_Manager. This should go in the BP constructor."))
	void InitializeManager(UPARAM(ref) UUI_Window_BaseOptions*& optionsUI);

	///<summary>Navigates the UI system. Returns false if currentUI and defaultUI are the same.</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI | Initialization", meta = (Tooltip = "Enables this UI_Manager to handle UI Navigation. Should be connected to Enhanced User Input in BP. Returns False if the current UI and the default UI are the same. This should be setup to quit the application."))
	bool NavigateWindows();

public:
	///<summary>Sets the currentUI to the defaultUI.</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI", meta = (Tooltip = "Sets the currentUI to the defaultUI."))
	void SetCurrentUIToDefault();

	///<summary>Sets currentUI to the UI_Window with the given UIID.</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI", meta = (Tooltip = "Sets the currentUI variable to a UI_Window that matches the given UUID and swaps the focus to the given UI."))
	void SetCurrentUIFromUIID(EUIID UIID);
};
