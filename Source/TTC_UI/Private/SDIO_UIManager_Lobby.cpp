// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIManager_Lobby.h"

void USDIO_UIManager_Lobby::NativeConstruct()
{
	Super::NativeConstruct();

	//Checks if objects are loaded
	if (Lobby_Window && Options_Window)
	{
		InitializeManager({ Lobby_Window, Options_Window });
	}
}
