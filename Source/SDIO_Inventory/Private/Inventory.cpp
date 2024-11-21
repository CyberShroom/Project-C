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

USDIO_Item* UInventory::GetItemFromInventory(FGuid itemID)
{
	for(USDIO_Item* element : inventoryList)
	{
		//Return the item if the id matches
		if (element->instanceID == itemID)
		{
			return element;
		}
	}

	//No item was found
	return nullptr;
}

void UInventory::RemoveItemFromInventory(FGuid itemID)
{
	USDIO_Item* itemToRemove = nullptr;

	for (USDIO_Item* element : inventoryList)
	{
		//Return the item if the id matches
		if (element->instanceID == itemID)
		{
			itemToRemove = element;
			break;
		}
	}

	//Stop if no item was found
	if (itemToRemove == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("An item with the GUID %s could not be found."), *itemID.ToString());
		return;
	}

	//Remove the item
	inventoryList.Remove(itemToRemove);
}
