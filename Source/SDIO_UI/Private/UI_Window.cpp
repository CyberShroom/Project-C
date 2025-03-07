// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Window.h"

EUIID UUI_Window::GetUIID()
{
	return UIID;
}

bool UUI_Window::GetAllowSubMenus()
{
	return bAllowSubMenus;
}

void UUI_Window::NavigateWindow()
{
	UE_LOG(LogTemp, Warning, TEXT("A UI Window tried to run NavigateWindow(), but the function either isn't overriden, or the Super event is being called. Please find the window and override this function / Remove the call to Super."));
	OnSetCurrentUIToDefault.Broadcast();
}

void UUI_Window::InitializeWindow()
{
	UE_LOG(LogTemp, Warning, TEXT("A UI Window tried to initialize, but the function either isn't overriden, or the Super event is being called. Please find the window and override this function / Remove the call to Super."));
	UIID = EUIID::NoID;
	bAllowSubMenus = false;
}
