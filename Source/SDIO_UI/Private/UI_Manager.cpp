// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Manager.h"


void UUI_Manager::SetCurrentUIFromUIID(EUIID UIID)
{
	//Placeholder window
	UUI_Window* window;

	//Checks if the UIID exists and sets it to window. Then sets currentUI to window.
	if (GetUIFromUIID(UIID, window))
	{
		currentUI->SetVisibility(ESlateVisibility::Hidden);
		window->SetVisibility(ESlateVisibility::Visible);

		currentUI = window;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The UI_Manager could not find a UI_Window with the UIID: %s."), *UEnum::GetValueAsString(UIID));
	}
}

bool UUI_Manager::GetUIFromUIID(EUIID UIID, UUI_Window*& window)
{
	//For each UI_Window controlled by the UI_Manager
	for(UUI_Window*& item : UIList)
	{
		//If the UIID matches the given UIID, return it
		if (item->GetUIID() == UIID)
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
	if (currentUI->GetAllowSubMenus())
	{
		currentUI->NavigateWindow();
	}
	else
	{
		//If the currentUI is the same as the defaultUI, return false. Otherwise, set the currentUI to the defaultUI.
		if (currentUI->GetUIID() == defaultUI)
		{
			return false;
		}
		else
		{
			SetCurrentUIToDefault();
		}
	}

	return true;
}

void UUI_Manager::InitializeManager(TArray<UUI_Window*> newUIs)
{
	//Load the UI Windows into the list
	InitializeUIsToList(newUIs);
	
	//Placeholder for the default ui
	UUI_Window* defaultWindow;

	//Checks if the defaultUI is set and loaded. If not, the game will create an error.
	if (GetUIFromUIID(defaultUI, defaultWindow))
	{
		currentUI = defaultWindow;
	}
	else
	{
		if (defaultUI == EUIID::NoID)
		{
			UE_LOG(LogTemp, Warning, TEXT("The UI Manager's default UI is not set!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("The UI_Manager could not find the default UI, %s, in the UIList."), *UEnum::GetValueAsString(defaultUI));
		}
	}
}

void UUI_Manager::InitializeUIsToList(TArray<UUI_Window*> newUIs)
{
	for (UUI_Window* item : newUIs)
	{
		//Initialize the UI
		item->InitializeWindow();

		//Do not add a UI that does not have its id set
		if (item->GetUIID() == EUIID::NoID)
		{
			UE_LOG(LogTemp, Error, TEXT("One of the UI Windows you just tried to add to the UI List doesn't have its ID set! It wont be added to the list."));
			continue;
		}

		//Add the window to the list
		UIList.Add(item);

		//For each UI_Window controlled by this UI_Manager, subscribe to their events
		item->OnSetCurrentUI.AddUniqueDynamic(this, &UUI_Manager::SetCurrentUIFromUIID);
		item->OnSetCurrentUIToDefault.AddUniqueDynamic(this, &UUI_Manager::SetCurrentUIToDefault);
	}
}

void UUI_Manager::SetCurrentUIToDefault()
{
	SetCurrentUIFromUIID(defaultUI);
}

