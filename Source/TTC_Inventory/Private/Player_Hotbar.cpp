// Fill out your copyright notice in the Description page of Project Settings.


#include "player_Hotbar.h"

void UPlayer_Hotbar::FillList(UWidget* parent)
{
	//Get the uniform grid. This cast shouldn't produce errors due to BeginPlay.
	UUniformGridPanel* panel = Cast<UUniformGridPanel>(parent);
	if (!IsValid(panel))
	{
		UE_LOG(LogTemp, Error, TEXT("FillList() in Hotbar_Inventory failed to cast to UUniformGridPanel! Hotbar will fail to fill!"));
		return;
	}

	//While the list is smaller than the current size and max size
	while (slotList.Num() < maxSize && slotList.Num() < currentSize)
	{
		//Create an item slot
		UButton_Item_Slot* newSlot1 = CreateWidget<UButton_Item_Slot>(parent, widgetRef);
		UButton_Item_Slot* newSlot2 = CreateWidget<UButton_Item_Slot>(parent, widgetRef);

		//Check if the slot was created successfully
		if (newSlot1 && newSlot2)
		{
			//Add it as a child of the panel and set its button event
			newSlot1->type = ESlotType::Projectile;
			newSlot2->type = ESlotType::Weapon;

			panel->AddChildToUniformGrid(newSlot1, 0, slotList.Num() / 2);
			panel->AddChildToUniformGrid(newSlot2, 1, slotList.Num() / 2);
			newSlot1->OnButtonClicked.AddUniqueDynamic(this, &UUIInventory::DelegateOnClickHandler);
			newSlot2->OnButtonClicked.AddUniqueDynamic(this, &UUIInventory::DelegateOnClickHandler);

			slotList.Add(newSlot1); //Add it to the list
			slotList.Add(newSlot2); //Add it to the list
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Player Hotbar failed to create an Item Slot Widget for the hotbar. Is widgetRef set correctly?"))
			break;
		}
	}
}
