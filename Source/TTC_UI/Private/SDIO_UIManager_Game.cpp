// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIManager_Game.h"

void USDIO_UIManager_Game::NativeConstruct()
{
	Super::NativeConstruct();

	//Checks if objects are loaded
	if (Inventory_Window && Options_Window)
	{
		UIList.Add(Inventory_Window);
		UIList.Add(Cast<UUI_Window>(Options_Window));

		InitializeManager(Options_Window);
	}
}
