// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Manager.h"
#include "UI_Window.h"


void UUI_Manager::SetCurrentUIFromUIID(EUIID UIID)
{
	UUI_Window* window;

	//Checks if the UIID exists and sets it to window. Then sets currentUI to window.
	if (GetUIFromUIID(UIID, window))
	{
		UWidget* windowWidget = window;
		UWidget* currentWidget = currentUI;

		SwapUIFocus(currentWidget, windowWidget);
		currentUI = window;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The UI_Manager could not find a UI_Window with the UIID: %s. Returned %s to prevent crashing."), *UEnum::GetValueAsString(UIID), *UEnum::GetValueAsString(currentUI->getUIID()));
	}
}

bool UUI_Manager::GetUIFromUIID(EUIID UIID, UUI_Window*& window)
{
	//For each UI_Window controlled by the UI_Manager
	for(UUI_Window*& item : UIList)
	{
		//If the UIID matches the given UIID, return it
		if (item->getUIID() == UIID)
		{
			window = item;
			return true;
		}
	}
	//If no UIID matches, set window to currentUI and return false
	window = currentUI;
	return false;
}

bool UUI_Manager::NavigateWindows()
{
	//Checks if the UI_Window is allowed to override Navigation
	if (currentUI->canOverrideNav())
	{
		currentUI->NavigateWindow();
	}
	else
	{
		//If the currentUI is the same as the defaultUI, return false. Otherwise, set the currentUI to the defaultUI.
		if (currentUI->getUIID() == defaultUI)
		{
			return false;
		}
		else
		{
			SetCurrentUIFromUIID(defaultUI);
		}
	}

	return true;
}

void UUI_Manager::InitializeManager(USDIO_UIWindow_Options*& optionsUI)
{
	UUI_Window* defaultWindow;

	//If optionsUI is loaded, set its defaultUI reference
	if (IsValid(optionsUI))
	{
		optionsUI->defaultUI = defaultUI;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The UI_Manager could not find the options UI. This can be safely ignored."));
	}

	//For each UI_Window controlled by this UI_Manager, subscribe to their events
	for (UUI_Window* item : UIList)
	{
		item->OnSetCurrentUI.AddUniqueDynamic(this, &UUI_Manager::SetCurrentUIFromUIID);
		item->OnSetCurrentUIToDefault.AddUniqueDynamic(this, &UUI_Manager::SetCurrentUIToDefault);
	}
	
	//Checks if the defaultUI is set and loaded. If not, the game will create an error.
	if (GetUIFromUIID(defaultUI, defaultWindow))
	{
		currentUI = defaultWindow;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("The UI_Manager could not find the default UI, %s, in the UIList."), *UEnum::GetValueAsString(defaultUI));
	}
}

void UUI_Manager::SetCurrentUIToDefault()
{
	SetCurrentUIFromUIID(defaultUI);
}

