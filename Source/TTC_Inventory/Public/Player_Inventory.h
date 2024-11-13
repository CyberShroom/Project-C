// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIInventory.h"
#include "Net/UnrealNetwork.h"
#include "SDIO_Item.h"
#include "Item_Slot.h"
#include "Components/UniformGridPanel.h"
#include "Button_Item_Slot.h"
#include "Player_Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckForSwap, UButton_Item_Slot*, slot);

UCLASS(meta = (ShortToolTip = "UI Inventory object. Contains the ui inventory and all logic associated with it."))
class TTC_INVENTORY_API UPlayer_Inventory : public UUIInventory
{
	GENERATED_BODY()

private:
	/// <summary>
	/// The number of slots per inventory row.
	/// </summary>
	UPROPERTY()
	uint8 slotsPerRow = 4;

	UFUNCTION()
	void OnClickHandler(UButton_Item_Slot* clickedSlot);

protected:
	/// <summary>
	/// Fills the item_slot list with item_slots. Stops at currentSize or maxSize if currentSize exceeds it.
	/// </summary>
	virtual void FillList(UWidget* parent) override;

public:
	/// <summary>
	/// Event Signature. Meant to be called when the button is clicked.
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "TTC_Inventory | Events", meta = (Tooltip = "An event that, when called, checks if a swap has occurred."))
	FCheckForSwap CheckForSwapEvent;

	/// <summary>
	/// The number of rows in the inventory
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The number of rows in the inventory."))
	uint8 currentRows = 0;

	/// <summary>
	/// The item contained in the mouse.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The item contained in the mouse."))
	USDIO_Item* mouseItem = nullptr;

	/// <summary>
	/// Handles logic on how the inventory is sorted. DO NOT EXPOSE TO BP!
	/// </summary>
	UFUNCTION()
	void HandleInventoryManagement(UButton_Item_Slot* clickedSlot, USDIO_Item* newItem);
};

//////////////////////////////////////////////////////////
// Create an inventory object							//
// Create a ui inventory object							//
// Both objects need to know the existence of the other	//
// Inventory is stored in player controller				//
// UI_Inventory is stored in the inventory ui			//
//////////////////////////////////////////////////////////
