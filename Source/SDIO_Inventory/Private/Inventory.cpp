// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

void UInventory::Initialize(uint8 maxInventorySize, uint8 currentInventorySize, EInventoryID id)
{
	maxSize = maxInventorySize;
	currentSize = currentInventorySize;
	inventoryID = id;
}

void UInventory::AddItemToInventory(USDIO_Item* newItem, bool bIsPickup)
{
	//Adds the item if theres space
	if (inventoryList.Num() < maxSize && inventoryList.Num() < currentSize)
	{
		inventoryList.Add(newItem);

		//Only run if the item is being picked up from world
		if (bIsPickup)
		{
			OnPickupItem.Broadcast(newItem);
		}
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

EInventoryID UInventory::GetInventoryID()
{
	return inventoryID;
}
