// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Title.h"

void USDIO_UIWindow_Title::DelegateOpenMultiplayer()
{
	OnSetCurrentUI.Broadcast(EUIID::MultiplayerUI);
}

void USDIO_UIWindow_Title::DelegateOpenOptions()
{
	OnSetCurrentUI.Broadcast(EUIID::OptionsUI);
}

void USDIO_UIWindow_Title::InitializeWindow()
{
	UIID = EUIID::TitleUI;
	bAllowSubMenus = false;
}

void USDIO_UIWindow_Title::NativeConstruct()
{
	Super::NativeConstruct();

	//Checks that the buttons are loaded
	if (Multiplayer && Options)
	{
		Multiplayer->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Title::DelegateOpenMultiplayer);
		Options->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Title::DelegateOpenOptions);
	}
}
