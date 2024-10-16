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

void USDIO_UIWindow_Inventory::SetInventorySize()
{
	uint8 currentRows = 0;

	while (inventorySlotList.Num() < maxInventorySize && inventorySlotList.Num() < currentInventorySize)
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

void USDIO_UIWindow_Inventory::SetHotbarSize()
{
	while (hotbarSlotList.Num() / 2 < maxHotbarSize && hotbarSlotList.Num() / 2 < currentHotbarSize)
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
	UE_LOG(LogTemp, Warning, TEXT("Detected Call from server."));
	for (UInventory_Slot* itemSlot : inventorySlotList)
	{
		if (itemSlot->GetContainedItem())
		{
			UE_LOG(LogTemp, Warning, TEXT("Contains an item."));
			continue;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NO ITEM. Setting contained item."));
			itemSlot->SetContainedItem(newItem, itemSlot->GetSprite());
			break;
		}
	}
}

void USDIO_UIWindow_Inventory::SetController()
{
	ABase_Player_Controller* controller = GetOwningPlayer<ABase_Player_Controller>();

	if (IsValid(controller))
	{
		owningController = controller;
		owningController->OnPickupItem.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::AddItemToUIInventory);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USDIO_UIWindow_Inventory::SetController);
	}
}

void USDIO_UIWindow_Inventory::SetInventoryPanel()
{
	UCanvasPanelSlot* panel = Cast<UCanvasPanelSlot>(Inventory->Slot);

	if (IsValid(panel))
	{
		inventoryPanelSlotReference = panel;
		slotMargin = inventoryPanelSlotReference->GetOffsets();
		SetInventorySize();
		SetHotbarSize();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &USDIO_UIWindow_Inventory::SetInventoryPanel);
	}
}

void USDIO_UIWindow_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	UIID = EUIID::InventoryUI;

	SetController();
	SetInventoryPanel();
}
