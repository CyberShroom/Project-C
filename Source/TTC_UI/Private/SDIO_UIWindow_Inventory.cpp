// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Inventory.h"

void USDIO_UIWindow_Inventory::DelegateCheckForSwap(UButton_Item_Slot* clickedSlot)
{
	//If the inventory cotains a mouse Item
	if (playerInventory->mouseItem)
	{
		//SWAP HAS OCCURED
		//playerInventory->HandleInventoryManagement(clickedSlot, INSERT ITEM HERE);
	}
	else
	{
		//Inventory does not contain a mouse Item
		playerInventory->HandleInventoryManagement(clickedSlot, nullptr);
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

void USDIO_UIWindow_Inventory::SetHotbarSize()
{
	while (hotbarSlotList.Num() / 2 < maxHotbarSize && hotbarSlotList.Num() / 2 < currentHotbarSize)
	{
		UButton_Item_Slot* newSlot1 = CreateWidget<UButton_Item_Slot>(GetWorld(), subWidget);
		UButton_Item_Slot* newSlot2 = CreateWidget<UButton_Item_Slot>(GetWorld(), subWidget);

		if (newSlot1 && newSlot2)
		{
			newSlot1->type = ESlotType::Projectile;
			newSlot2->type = ESlotType::Weapon;

			Hotbar->AddChildToUniformGrid(newSlot1, 0, hotbarSlotList.Num() / 2);
			Hotbar->AddChildToUniformGrid(newSlot2, 1, hotbarSlotList.Num() / 2);

			//newSlot1->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::MoveInventoryItems);
			//newSlot2->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::MoveInventoryItems);

			hotbarSlotList.Add(newSlot1);
			hotbarSlotList.Add(newSlot2);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inventory Window failed to create an Inventory Slot Widget for the hotbar. Is subWidget set in the BP config?"))
			break;
		}
	}
}

void USDIO_UIWindow_Inventory::InitializeAttributes(UInventory* invRef)
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

		//Set the inventory panel size (Initialize contains the code for filling the inventory so this should run afterwards)
		SetInventoryPanelSize();
		SetHotbarSize();
	}
	else
	{
		//If not, try again next frame
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, invRef]() {InitializeAttributes(invRef); });
	}
}

void USDIO_UIWindow_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	playerInventory = NewObject<UPlayer_Inventory>();
}
