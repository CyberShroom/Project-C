// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_Slot.h"

//void UInventory_Slot::updateSprite()
//{
//	SetSprite(Sprite, );
//}

void UInventory_Slot::ButtonClicked()
{
	OnButtonClicked.Broadcast(this);
}

UImage* UInventory_Slot::GetSprite()
{
	return Sprite;
}

void UInventory_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button && Sprite)
	{
		Button->OnClicked.AddUniqueDynamic(this, &UInventory_Slot::ButtonClicked);
	}
}
