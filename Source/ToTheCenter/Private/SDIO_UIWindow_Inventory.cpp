// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Inventory.h"

void USDIO_UIWindow_Inventory::MoveInventoryItems(UInventory_Slot* clickedSlot)
{
	UTTC_Item* containedItem = clickedSlot->GetContainedItem();
	if (mouseItem)
	{
		if (containedItem)
		{
			SwapInventoryItems(clickedSlot, containedItem, mouseItem);
		}
		else
		{
			SwapInventoryItems(clickedSlot, nullptr, mouseItem);
		}
	}
	else
	{
		SwapInventoryItems(clickedSlot, containedItem, nullptr);
	}
}

void USDIO_UIWindow_Inventory::SwapInventoryItems(UInventory_Slot* clickedSlot, UTTC_Item* slotItem, UTTC_Item* newItem)
{
	clickedSlot->SetContainedItem(newItem, clickedSlot->GetSprite());
	mouseItem = slotItem;
}

void USDIO_UIWindow_Inventory::SetInventorySize(uint8 size)
{
	uint8 currentRows = 0;

	while (inventorySlotList.Num() < owningController->GetMaxInventorySize() && inventorySlotList.Num() < size)
	{
		UInventory_Slot* newSlot = CreateWidget<UInventory_Slot>(GetWorld(), subWidget);

		if (newSlot)
		{
			Inventory->AddChildToUniformGrid(newSlot, FMath::FloorToInt((float) inventorySlotList.Num() / slotsPerRow), inventorySlotList.Num() % slotsPerRow);
			Inventory->AddChildToUniformGrid(newSlot, FMath::FloorToInt((float)inventorySlotList.Num() / slotsPerRow), inventorySlotList.Num() % slotsPerRow);
			newSlot->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::MoveInventoryItems);

			inventorySlotList.Add(newSlot);

			currentRows = FMath::CeilToInt((float)inventorySlotList.Num() / slotsPerRow);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Inventory Window failed to create an Inventory Slot Widget for the inventory. Is subWidget set in the BP config?"))
			break;
		}
	}

	if (currentRows != 0)
	{
		slotMargin.Top = 460 - (70 * currentRows);
		inventoryPanelSlotReference->SetOffsets(slotMargin);
	}
}

void USDIO_UIWindow_Inventory::SetHotbarSize(uint8 size)
{
	while (hotbarSlotList.Num() / 2 < 4 && hotbarSlotList.Num() / 2 < size)
	{
		UInventory_Slot* newSlot1 = CreateWidget<UInventory_Slot>(GetWorld(), subWidget);
		UInventory_Slot* newSlot2 = CreateWidget<UInventory_Slot>(GetWorld(), subWidget);

		if (newSlot1 && newSlot2)
		{
			newSlot1->type = ESlotType::Projectile;
			newSlot2->type = ESlotType::Weapon;

			Hotbar->AddChildToUniformGrid(newSlot1, 0, hotbarSlotList.Num() / 2);
			Hotbar->AddChildToUniformGrid(newSlot2, 1, hotbarSlotList.Num() / 2);

			newSlot1->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::MoveInventoryItems);
			newSlot2->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::MoveInventoryItems);

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

void USDIO_UIWindow_Inventory::AddItemToUIInventory(UTTC_Item* newItem)
{
	for (UInventory_Slot* itemSlot : inventorySlotList)
	{
		if (itemSlot->GetContainedItem())
		{
			continue;
		}
		else
		{
			itemSlot->SetContainedItem(newItem, itemSlot->GetSprite());
			break;
		}
	}
}

void USDIO_UIWindow_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	UIID = EUIID::InventoryUI;

	inventoryPanelSlotReference = Cast<UCanvasPanelSlot>(Inventory->Slot);
	if (inventoryPanelSlotReference)
	{
		slotMargin = inventoryPanelSlotReference->GetOffsets();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SDIO_UIWindow_Inventory failed to cast Uniform Grid Slot to Canvas Panel. Is the grid a child of the canvas panel? Is there a canvas panel?"))
	}

	if (ABase_Player_Controller* castResult = Cast<ABase_Player_Controller>(GetOwningLocalPlayer()->GetPlayerController(GetWorld())))
	{
		owningController = castResult;
		owningController->OnPickupItem.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::AddItemToUIInventory);

		SetInventorySize(owningController->currentInventorySize);
		SetHotbarSize(2);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SDIO_UIWindow_Inventory could not find the owning player controller."))
	}
}
