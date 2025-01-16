// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Inventory.h"

void USDIO_UIWindow_Inventory::DelegateInventoryInteraction(UButton_Item_Slot* clickedSlot, EInventoryID id)
{
	// Flag that determines if id should be the next last inventory interaction at end of function
	bool bUpdateInteraction = false;

	//If clickSlot contains an item, thats an interaction. Set flag to true.
	if (clickedSlot->GetContainedItem())
	{
		bUpdateInteraction = true;
	}

	//Do different things based on the last interacted inventory. Remember that clicking an empty slot does not count as an interaction.
	switch (lastInteractedInventory)
	{
		case EInventoryID::NOID: //No interacted inventory, handle from inventory.
			ReturnInventoryInteraction(clickedSlot, nullptr, id);
			break;
		case EInventoryID::Main_Inventory:
			//If ids are the same, no swap occurs
			if (lastInteractedInventory == id)
			{
				ReturnInventoryInteraction(clickedSlot, nullptr, id);
			}
			else //Since lastinteractedinventory only updates on clicking a slot with an item in it, a swap has to occur.
			{
				ReturnInventoryInteraction(clickedSlot, playerInventory->mouseItem, id);
				onMoveItemBetweenInventories.Broadcast(playerInventory->mouseItem->instanceID, id, lastInteractedInventory);
				playerInventory->mouseItem = nullptr;
			}
			break;
		case EInventoryID::Hotbar_Inventory:
			//If ids are the same, no swap occurs
			if (lastInteractedInventory == id)
			{
				ReturnInventoryInteraction(clickedSlot, nullptr, id);
			}
			else //Since lastinteractedinventory only updates on clicking a slot with an item in it, a swap has to occur.
			{
				ReturnInventoryInteraction(clickedSlot, hotbarInventory->mouseItem, id);
				onMoveItemBetweenInventories.Broadcast(hotbarInventory->mouseItem->instanceID, id, lastInteractedInventory);
				hotbarInventory->mouseItem = nullptr;
			}
			break;
		case EInventoryID::Equipment_Inventory:
			//If id is equal
				//HandleInventory
			//Else
				//Swap has occurred
			break;
		case EInventoryID::Passive_Inventory:
			//ignore for now
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Inventory Interaction Event From UIWindow_Inventory failed to recieve a correct ID."));
			break;
	}

	//If flag is true, update last interaction
	if (bUpdateInteraction)
	{
		lastInteractedInventory = id;
	}
	else //Otherwise clear last interaction
	{
		lastInteractedInventory = EInventoryID::NOID;
	}
}

void USDIO_UIWindow_Inventory::ReturnInventoryInteraction(UButton_Item_Slot* clickedSlot, USDIO_Item* swapItem, EInventoryID id)
{
	//Runs the inventory handler for the given id and only the given id
	switch (id)
	{
		case EInventoryID::Main_Inventory:
			playerInventory->HandleInventoryManagement(clickedSlot, swapItem);
			break;
		case EInventoryID::Hotbar_Inventory:
			hotbarInventory->HandleInventoryManagement(clickedSlot, swapItem);
			break;
		case EInventoryID::Equipment_Inventory:
			break;
		case EInventoryID::Passive_Inventory:
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Return Inventory Interaction Event From UIWindow_Inventory failed to recieve a correct ID."));
			break;
	}
}

void USDIO_UIWindow_Inventory::SetInventoryPanelSize()
{
	if (playerInventory->currentRows != 0)
	{
		slotMargin.Top = 460 - (70 * playerInventory->currentRows);
		inventoryPanelSlotReference->SetOffsets(slotMargin);
	}
}

void USDIO_UIWindow_Inventory::InitializeAttributes(UInventory* invRef, UInventory* hotbarRef)
{
	//Attempt tp cast the Inventory panel
	UCanvasPanelSlot* panel = Cast<UCanvasPanelSlot>(Inventory->Slot);

	//Check if it succeeded
	if (IsValid(panel))
	{
		//On Success

		//Set the panelslot reference and the slotMargin reference
		inventoryPanelSlotReference = panel;
		slotMargin = inventoryPanelSlotReference->GetOffsets();

		//Initialize the player inventory
		playerInventory->Initialize(24, 8, invRef, Inventory, subWidget, EInventoryID::Main_Inventory);
		playerInventory->InventoryInteractionEvent.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::DelegateInventoryInteraction);

		//Initialize the player hotbar
		hotbarInventory->Initialize(8, 8, hotbarRef, Hotbar, subWidget, EInventoryID::Hotbar_Inventory);
		hotbarInventory->InventoryInteractionEvent.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::DelegateInventoryInteraction);

		//Set the inventory panel size (Initialize contains the code for filling the inventory so this should run afterwards)
		SetInventoryPanelSize();
	}
	else
	{
		//If not, try again next frame
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, invRef, hotbarRef]() {InitializeAttributes(invRef, hotbarRef); });
	}
}

void USDIO_UIWindow_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	playerInventory = NewObject<UPlayer_Inventory>();
	hotbarInventory = NewObject<UPlayer_Hotbar>();
}
