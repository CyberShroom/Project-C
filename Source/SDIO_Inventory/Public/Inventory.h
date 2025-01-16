// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "SDIO_Item.h"
#include "SDIO_Inventory_Enums.h"
#include "Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupItem, USDIO_Item*, pickedItem);

UCLASS(meta = (ShortToolTip = "Inventory object. Contains the inventory and all logic associated with it."))
class SDIO_INVENTORY_API UInventory : public UObject
{
	GENERATED_BODY()

private:
	/// <summary>
	/// Max size of the inventory
	/// </summary>
	UPROPERTY()
	uint8 maxSize = 0;

	/// <summary>
	/// The inventory
	/// </summary>
	UPROPERTY()
	TArray<USDIO_Item*> inventoryList;

	/// <summary>
	/// The ID of the inventory
	/// </summary>
	UPROPERTY()
	EInventoryID inventoryID = EInventoryID::NOID;

public:
	/// <summary>
	/// Event Signature. Meant to be called when an item is attempted to be inserted into the inventory.
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "SDIO_Inventory | Events", meta = (Tooltip = "Event Signature. Meant to be called when an item is attempted to be inserted into the inventory."))
	FPickupItem OnPickupItem;

	/// <summary>
	/// Current allowed size of the inventory
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The current size of the inventory."))
	uint8 currentSize = 0;

	/// <summary>
	/// Inventory object initializer
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory | Initializers", meta = (Tooltip = "Initializes the inventory object. This must be ran after creating the object."))
	void Initialize(uint8 maxInventorySize, uint8 initialInventorySize, EInventoryID id);

	/// <summary>
	/// Adds the given item to the inventory
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Adds the given item to the inventory object."))
	void AddItemToInventory(USDIO_Item* newItem, bool bIsNew);

	/// <summary>
	/// Returns an item with the given GUID or nullptr if it doesn't exist
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Returns an item with the given GUID. Returns nullptr if it wasn't found."))
	USDIO_Item* GetItemFromInventory(FGuid itemID);

	/// <summary>
	/// Removes the given item from the inventory
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Removes the given item from the inventory."))
	void RemoveItemFromInventory(FGuid itemID);

	/// <summary>
	/// Returns the inventory ID
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Returns the ID of this inventory object"))
	EInventoryID GetInventoryID();
};

//////////////////////////////////////////////////////////
// Create an inventory object							//
// Create a ui inventory object							//
// Both objects need to know the existence of the other	//
// Inventory is stored in player controller				//
// UI_Inventory is stored in the inventory ui			//
//////////////////////////////////////////////////////////
