// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Inventory.h"

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
			newSlot->OnButtonClicked.AddUniqueDynamic(this, &UUIInventory::DelegateOnClickHandler);

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


