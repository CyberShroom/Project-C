// Fill out your copyright notice in the Description page of Project Settings.


#include "UIInventory.h"

void UUIInventory::Initialize(uint8 maxInventorySize, uint8 currentInventorySize, UInventory* inventoryRef, UWidget* panelRef, TSubclassOf<UItem_Slot> widget, EInventoryID id)
{
	widgetRef = widget;
	maxSize = maxInventorySize;
	currentSize = currentInventorySize;
	inventoryID = id;
	inventoryRef->OnPickupItem.AddUniqueDynamic(this, &UUIInventory::AddItemToUIInventory);

	FillList(panelRef);
}

void UUIInventory::AddItemToUIInventory(USDIO_Item* newItem)
{
	//Check for an empty slot and add the item to the first empty slot
	for (UItem_Slot* itemSlot : slotList)
	{
		if (itemSlot->GetContainedItem())
		{
			continue;
		}
		else
		{
			itemSlot->SetContainedItem(newItem);
			break;
		}
	}
}

EInventoryID UUIInventory::GetInventoryID()
{
	return inventoryID;
}

void UUIInventory::FillList(UWidget* parent)
{
	UE_LOG(LogTemp, Warning, TEXT("UI_Inventory attempted to call FillList() without override!"));
}


