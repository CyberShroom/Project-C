// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Options.h"

void USDIO_UIWindow_Options::NativeConstruct()
{
	Super::NativeConstruct();

	UIID = EUIID::OptionsUI;

	if (Exit && Quit && Audio && Controls)
	{
		Exit->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Options::ExitMenu);
	}
}

void USDIO_UIWindow_Options::ExitMenu()
{
	OnSetCurrentUIToDefault.Broadcast();
}
