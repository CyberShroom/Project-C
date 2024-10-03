// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Options.h"

void USDIO_UIWindow_Options::NativeConstruct()
{
	Super::NativeConstruct();

	UIID = EUIID::OptionsUI;
	overrideNav = true;

	//Checks if the objects are loaded
	if (Exit && Quit && Audio && Controls)
	{
		Audio->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Options::OpenAudioPanel);
		Controls->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Options::OpenControlsPanel);
		Exit->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Options::ExitMenu);
	}
}

void USDIO_UIWindow_Options::NavigateWindow()
{
	//Checks if a main menu or sub menu is focused
	if (windowID == 0)
	{
		OnSetCurrentUIToDefault.Broadcast();
	}
	else
	{
		OpenOptionsPanel();
		windowID = 0;
	}
}

void USDIO_UIWindow_Options::ExitMenu()
{
	OnSetCurrentUIToDefault.Broadcast();
}

void USDIO_UIWindow_Options::OpenAudioPanel()
{
	SwapUIFocus(ButtonBox, AudioPanel);
	windowID = 1;
}

void USDIO_UIWindow_Options::OpenControlsPanel()
{
	SwapUIFocus(ButtonBox, ControlsPanel);
	windowID = 1;
}

void USDIO_UIWindow_Options::OpenOptionsPanel()
{
	AudioPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsPanel->SetVisibility(ESlateVisibility::Hidden);

	ButtonBox->SetVisibility(ESlateVisibility::Visible);
}
