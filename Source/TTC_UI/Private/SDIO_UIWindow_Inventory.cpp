// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Inventory.h"

void USDIO_UIWindow_Inventory::DelegateCheckForSwap(UButton_Item_Slot* clickedSlot)
{
	//If the inventory cotains a mouse Item
	if (playerInventory->mouseItem)
	{
		//SWAP HAS OCCURED
		hotbarInventory->HandleInventoryManagement(clickedSlot, playerInventory->mouseItem);
		onMoveItemToHotbar.Broadcast(playerInventory->mouseItem->instanceID);
		playerInventory->mouseItem = nullptr;
	}
	else if(hotbarInventory->mouseItem)
	{
		//SWAP HAS OCCURED
		playerInventory->HandleInventoryManagement(clickedSlot, hotbarInventory->mouseItem);
		onMoveItemFromHotbar.Broadcast(hotbarInventory->mouseItem->instanceID);
		hotbarInventory->mouseItem = nullptr;
	}
	else
	{
		playerInventory->HandleInventoryManagement(clickedSlot, nullptr);
		hotbarInventory->HandleInventoryManagement(clickedSlot, nullptr);
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
		playerInventory->Initialize(24, 8, invRef, Inventory, subWidget);
		playerInventory->CheckForSwapEvent.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::DelegateCheckForSwap);

		//Initialize the player hotbar
		hotbarInventory->Initialize(8, 8, hotbarRef, Hotbar, subWidget);
		hotbarInventory->CheckForSwapEvent.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::DelegateCheckForSwap);

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
