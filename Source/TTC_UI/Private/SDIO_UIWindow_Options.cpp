// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Options.h"

void USDIO_UIWindow_Options::InitializeWindow()
{
	UIID = EUIID::OptionsUI;
	bAllowSubMenus = true;
}

void USDIO_UIWindow_Options::NativeConstruct()
{
	Super::NativeConstruct();

	//Checks if the objects are loaded
	if (Exit && Quit && Audio && Controls)
	{
		Audio->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Options::DelegateOpenAudioPanel);
		Controls->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Options::DelegateOpenControlsPanel);
		Exit->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Options::DelegateExitMenu);
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

void USDIO_UIWindow_Options::DelegateExitMenu()
{
	OnSetCurrentUIToDefault.Broadcast();
}

void USDIO_UIWindow_Options::DelegateOpenAudioPanel()
{
	ButtonBox->SetVisibility(ESlateVisibility::Hidden);
	AudioPanel->SetVisibility(ESlateVisibility::Visible);
	windowID = 1;
}

void USDIO_UIWindow_Options::DelegateOpenControlsPanel()
{
	ButtonBox->SetVisibility(ESlateVisibility::Hidden);
	ControlsPanel->SetVisibility(ESlateVisibility::Visible);
	windowID = 1;
}

void USDIO_UIWindow_Options::OpenOptionsPanel()
{
	AudioPanel->SetVisibility(ESlateVisibility::Hidden);
	ControlsPanel->SetVisibility(ESlateVisibility::Hidden);

	ButtonBox->SetVisibility(ESlateVisibility::Visible);
}
