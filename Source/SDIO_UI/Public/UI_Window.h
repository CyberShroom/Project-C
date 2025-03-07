// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDIO_UI_Enums.h"
#include "Blueprint/UserWidget.h"
#include "UI_Window.generated.h"

/**
 * C Class. Contains common fields and methods for UI Windows.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetCurrentUI, EUIID, UIID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetCurrentUIToDefault);

UCLASS(meta = (ShortToolTip = "C Class. Contains common fields and methods for UI Windows."))
class SDIO_UI_API UUI_Window : public UUserWidget
{
	GENERATED_BODY()

protected:
	///<summary>
	/// How many layers deep are the submenus? Serves no purpose if overrideNav is false. Could be used for other things but not recommended.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information", meta = (Tooltip = "A uint8 ID system designed to be used to navigate submenus."))
	uint8 windowID = 0;

	///<summary>
	/// The UIID of the window. Used to identify this object.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The UIID of the UI_Window."))
	EUIID UIID = EUIID::NoID;

	///<summary>
	/// Should this menu be allowed to have submenus? Must be true to use the submenu system. Make sure to override NavigateWindow or this does nothing.
	///</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "Should this menu be allowed to have submenus? Must be true to use the submenu system. Make sure to override NavigateWindow or this does nothing."))
	bool bAllowSubMenus = false;

public:
	/// <summary>
	/// Returns the UI Windows UIID.
	/// </summary>
	UFUNCTION(BlueprintPure, Category = "SDIO_UI | Getters", meta = (Tooltip = "Returns the UI Windows UIID."))
	EUIID GetUIID();

	/// <summary>
	/// Returns bAllowSubMenus.
	/// </summary>
	UFUNCTION(BlueprintPure, Category = "SDIO_UI | Getters", meta = (Tooltip = "Returns bAllowSubMenus."))
	bool GetAllowSubMenus();

	///<summary>
	/// Called when the UI Window wants to change the viewport to another UI Window.
	///</summary>
	UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Events", meta = (Tooltip = "Called when the UI Window wants to change the viewport to another UI Window."))
	FSetCurrentUI OnSetCurrentUI;

	///<summary>
	/// Called when the UI Window wants to change the viewport to the UI Managers default UI.
	///</summary>
	UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Events", meta = (Tooltip = "Called when the UI Window wants to change the viewport to the UI Managers default UI."))
	FSetCurrentUIToDefault OnSetCurrentUIToDefault;

	///<summary>
	/// Allows the window to navigate within itself. Does nothing if bAllowSubMenus is false. Must Be Overridden or it will cause a warning.
	///</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI", meta = (Tooltip = "Contains logic to navigate the UI_Window. Only used if the window contains submenus or needs special code."))
	virtual void NavigateWindow();

	/// <summary>
	/// Sets the default values of this instance.
	/// </summary>
	UFUNCTION()
	virtual void InitializeWindow();
};
