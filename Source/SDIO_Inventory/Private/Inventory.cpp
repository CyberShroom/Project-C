// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

void UInventory::Initialize(uint8 maxInventorySize, uint8 currentInventorySize)
{
	maxSize = maxInventorySize;
	currentSize = currentInventorySize;
}

void UInventory::AddItemToInventory(USDIO_Item* newItem)
{
	//Adds the item if theres space
	if (inventoryList.Num() < maxSize && inventoryList.Num() < currentSize)
	{
		inventoryList.Add(newItem);
		OnPickupItem.Broadcast(newItem);
	}
}
