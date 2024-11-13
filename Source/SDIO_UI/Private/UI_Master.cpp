// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Master.h"

void UUI_Master::SwapUIFocus(UWidget*& oldWidget, UWidget*& newWidget)
{
	oldWidget->SetVisibility(ESlateVisibility::Hidden);
	newWidget->SetVisibility(ESlateVisibility::Visible);
}
