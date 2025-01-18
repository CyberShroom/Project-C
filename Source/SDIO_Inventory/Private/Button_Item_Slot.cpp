// Fill out your copyright notice in the Description page of Project Settings.


#include "Button_Item_Slot.h"

void UButton_Item_Slot::DelegateButtonClicked()
{
	OnButtonClicked.Broadcast(this);
}

void UButton_Item_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button && Sprite)
	{
		Button->OnClicked.AddUniqueDynamic(this, &UButton_Item_Slot::DelegateButtonClicked);
	}
}


