// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Master.h"
#include "SDIO_UI_Enums.h"
#include "UI_Window.generated.h"


class UUI_Manager; //Forward Declaration
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetCurrentUI, EUIID, UIID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetCurrentUIToDefault);

UCLASS()
class TOTHECENTER_API UUI_Window : public UUI_Master
{
	GENERATED_BODY()

private:
	UUI_Manager* managerReference; //Forwrad Declaration, please ignore

protected:
	///<summary>The UIID of the window. Used to identify this object.</summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The UIID of the UI_Window."))
	EUIID UIID = EUIID::NoID;

	///<summary>Should this window override the UI_Managers Navigation system? Should be true if it contains submenus that require navigation.</summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "Controls whether this UI_Window should take control of UI Navigation."))
	bool overrideNav = false;

	///<summary>How many layers deep are the submenus? Serves no purpose if overrideNav is false. Could be used for other things but not recommended.</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information", meta = (Tooltip = "A uint8 ID system designed to be used to navigate submenus."))
	uint8 windowID = 0;

public:
	///<summary>Event Signature. Meant to be called for setting the UI_Manager's currentUI from within this window.</summary>
	UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Event Dispatchers", meta = (Tooltip = "An event that, when called, sets the UI_Managers currentUI to the given UI."))
	FSetCurrentUI OnSetCurrentUI;

	///<summary>Event Signature. Meant to be called for setting the UI_Manager's currentUI to its defaultUI from within this window.</summary>
	UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Event Dispatchers", meta = (Tooltip = "An event that, when called, sets the UI_Managers currentUI to it's defaultUI."))
	FSetCurrentUIToDefault OnSetCurrentUIToDefault;

	///<summary>Pure Function. Returns this window's UIID.</summary>
	UFUNCTION(BlueprintPure, Category = "SDIO_UI", meta = (Tooltip = "Returns the value of UIID."))
	EUIID getUIID();

	///<summary>Pure Function. Returns overrideNav.</summary>
	UFUNCTION(BlueprintPure, Category = "SDIO_UI", meta = (Tooltip = "Returns the value of overrideNav."))
	bool canOverrideNav();

	///<summary>Allows the window to navigate within itself. Does nothing if overrideNav is false. Must Be Overridden or it will cause a warning.</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI | Initialization", meta = (Tooltip = "Contains logic to navigate the UI_Window. Only used if the window contains submenus or needs special code."))
	virtual void NavigateWindow();
};
