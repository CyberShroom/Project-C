// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Title.h"

/*
void USDIO_UIWindow_Title::StartSingleplayer()
{
	UE_LOG(LogTemp, Log, TEXT("REMINDER: Multiplayer is currently handled by BP."));
}
*/

void USDIO_UIWindow_Title::DelegateOpenMultiplayer()
{
	OnSetCurrentUI.Broadcast(EUIID::MultiplayerUI);
}

void USDIO_UIWindow_Title::DelegateOpenOptions()
{
	OnSetCurrentUI.Broadcast(EUIID::OptionsUI);
}

/*
void USDIO_UIWindow_Title::ExitGame()
{
	UE_LOG(LogTemp, Log, TEXT("REMINDER: Quitting is currently handled by BP."));
}
*/

void USDIO_UIWindow_Title::NativeConstruct()
{
	Super::NativeConstruct();

	//Checks that the buttons are loaded
	if (Singleplayer && Multiplayer && Options && Quit)
	{
		//Singleplayer->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Title::StartSingleplayer);
		Multiplayer->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Title::DelegateOpenMultiplayer);
		Options->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Title::DelegateOpenOptions);
		//Quit->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Title::ExitGame);
	}
}
