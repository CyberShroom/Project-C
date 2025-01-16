// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Inventory.h"

void UPlayer_Inventory::OnClickHandler(UButton_Item_Slot* clickedSlot)
{
	InventoryInteractionEvent.Broadcast(clickedSlot, GetInventoryID());
}

void UPlayer_Inventory::HandleInventoryManagement(UButton_Item_Slot* clickedSlot, USDIO_Item* newItem)
{
	//Do nothing if the container doesn't exist in this array
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

void UPlayer_Inventory::FillList(UWidget* parent)
{
	//Get the uniform grid. This cast shouldn't produce errors due to BeginPlay.
	UUniformGridPanel* panel = Cast<UUniformGridPanel>(parent);
	if (!IsValid(panel))
	{
		UE_LOG(LogTemp, Error, TEXT("FillList() in Player_Inventory failed to cast to UUniformGridPanel! Inventory will fail to fill!"));
		return;
	}

	//While the list is smaller than the current size and max size
	while (slotList.Num() < maxSize && slotList.Num() < currentSize)
	{
		//Create an item slot
		UButton_Item_Slot* newSlot = CreateWidget<UButton_Item_Slot>(parent, widgetRef);

		//Check if the slot was created successfully
		if (newSlot)
		{
			//Add it as a child of the panel and set its button event
			panel->AddChildToUniformGrid(newSlot, FMath::FloorToInt((float)slotList.Num() / slotsPerRow), slotList.Num() % slotsPerRow);
			newSlot->OnButtonClicked.AddUniqueDynamic(this, &UPlayer_Inventory::OnClickHandler);

			slotList.Add(newSlot); //Add it to the list

			currentRows = FMath::CeilToInt((float)slotList.Num() / slotsPerRow);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Player Inventory failed to create an Item Slot Widget for the inventory. Is widgetRef set correctly?"))
			break;
		}
	}
}


