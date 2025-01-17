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

void UUIInventory::DelegateOnClickHandler(UButton_Item_Slot* clickedSlot)
{
	OnInventoryInteraction.Broadcast(clickedSlot, GetInventoryID());
}

void UUIInventory::HandleInventoryManagement(UButton_Item_Slot* clickedSlot, USDIO_Item* newItem)
{
	if (!slotList.Contains(clickedSlot)) return;

	if (mouseItem) //True if mouse contains an item
	{
		if (clickedSlot->GetContainedItem()) //True if there is a contained item
		{
			//True + True; swap the items.
			clickedSlot->SetContainedItem(mouseItem);
			mouseItem = clickedSlot->GetContainedItem();
		}
		else
		{
			//True + False; Place the mouse item into the container and remove the mouse item.
			clickedSlot->SetContainedItem(mouseItem);
			mouseItem = nullptr;
		}
	}
	else if (newItem) //True only if a swap has occurred
	{
		mouseItem = clickedSlot->GetContainedItem();
		clickedSlot->SetContainedItem(newItem);
	}
	else
	{
		//False + Assumed True; Place the item in the mouse and remove the item from the container
		mouseItem = clickedSlot->GetContainedItem();
		clickedSlot->SetContainedItem(nullptr);
	}
}
