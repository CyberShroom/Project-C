// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIManager_Title.h"

void USDIO_UIManager_Title::NativeConstruct()
{
	Super::NativeConstruct();

	defaultUI = EUIID::TitleUI;

	//Checks if objects are loaded
	if (Title_Window && Multiplayer_Window && Options_Window)
	{
		UIList.Add(Title_Window);
		UIList.Add(Multiplayer_Window);
		UIList.Add(Cast<UUI_Window>(Options_Window));

		InitializeManager(Options_Window);
	}
}
