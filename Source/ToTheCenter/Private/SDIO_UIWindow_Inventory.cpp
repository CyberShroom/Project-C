// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Inventory.h"

void USDIO_UIWindow_Inventory::MoveInventoryItems(UInventory_Slot* clickedSlot)
{
	if (mouseItem)
	{
		if (true)
		{

		}
	}
}

void USDIO_UIWindow_Inventory::SetInventorySize(uint8 size)
{
	while (inventorySlotList.Num() < owningController->GetMaxInventorySize() && inventorySlotList.Num() < size)
	{
		UInventory_Slot* newSlot = CreateWidget<UInventory_Slot>(Inventory, subWidget);

		newSlot->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::MoveInventoryItems);
	}
}

void USDIO_UIWindow_Inventory::SetHotbarSize(uint8 size)
{
	while (hotbarSlotList.Num() / 2 < 4 && hotbarSlotList.Num() / 2 < size)
	{
		UInventory_Slot* newSlot1 = CreateWidget<UInventory_Slot>(Hotbar, subWidget);
		UInventory_Slot* newSlot2 = CreateWidget<UInventory_Slot>(Hotbar, subWidget);

		newSlot1->type = ESlotType::Projectile;
		newSlot2->type = ESlotType::Weapon;

		newSlot1->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::MoveInventoryItems);
		newSlot2->OnButtonClicked.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::MoveInventoryItems);
	}
}

void USDIO_UIWindow_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	if (ABase_Player_Controller* castResult = Cast<ABase_Player_Controller>(GetOwningLocalPlayer()->GetPlayerController(GetWorld())))
	{
		owningController = castResult;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SDIO_UIWindow_Inventory could not find the owning player controller."))
	}
}
