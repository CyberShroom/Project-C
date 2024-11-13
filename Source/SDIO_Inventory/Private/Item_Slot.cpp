#include "Item_Slot.h"
// Fill out your copyright notice in the Description page of Project Settings.

USDIO_Item* UItem_Slot::GetContainedItem()
{
	return containedItem;
}

void UItem_Slot::SetContainedItem(USDIO_Item* newItem)
{
	containedItem = newItem;
}

void UItem_Slot::NativeConstruct()
{
	Super::NativeConstruct();
}
