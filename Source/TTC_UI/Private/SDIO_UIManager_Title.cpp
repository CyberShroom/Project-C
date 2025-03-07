// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIManager_Title.h"

void USDIO_UIManager_Title::NativeConstruct()
{
	Super::NativeConstruct();

	//Checks if objects are loaded
	if (Title_Window && Multiplayer_Window && Options_Window)
	{
		InitializeManager({Title_Window, Multiplayer_Window, Options_Window});
	}
}
