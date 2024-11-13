// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Window.h"


void UUI_Window::NavigateWindow()
{
	UE_LOG(LogTemp, Warning, TEXT("A UI_Window tried to run NavigateWindow(), but the function either isn't overriden, or the Super event is being called. Please find the window and override this function / Remove the call to Super."));
	OnSetCurrentUIToDefault.Broadcast();
}
