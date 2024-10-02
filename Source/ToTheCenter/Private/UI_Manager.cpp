// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Manager.h"
#include "UI_Window.h"


void UUI_Manager::SetCurrentUIFromUIID(EUIID UIID)
{
	UUI_Window* window;

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
	for(UUI_Window*& item : UIList)
	{
		if (item->getUIID() == UIID)
		{
			window = item;
			return true;
		}
	}
	window = currentUI;
	return false;
}

void UUI_Manager::InitializeManager(USDIO_UIWindow_Options*& optionsUI)
{
	UUI_Window* defaultWindow;

	if (IsValid(optionsUI))
	{
		optionsUI->defaultUI = defaultUI;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The UI_Manager could not find the options UI. This can be safely ignored."));
	}

	for (UUI_Window* item : UIList)
	{
		item->OnSetCurrentUI.AddUniqueDynamic(this, &UUI_Manager::SetCurrentUI);
		item->OnSetCurrentUIToDefault.AddUniqueDynamic(this, &UUI_Manager::SetCurrentUIToDefault);
	}
		
	if (GetUIFromUIID(defaultUI, defaultWindow))
	{
		currentUI = defaultWindow;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("The UI_Manager could not find the default UI, %s, in the UIList."), *UEnum::GetValueAsString(defaultUI));
	}
}

void UUI_Manager::SetCurrentUI(EUIID UIID)
{
	SetCurrentUIFromUIID(UIID);
}

void UUI_Manager::SetCurrentUIToDefault()
{
	SetCurrentUIFromUIID(defaultUI);
}

