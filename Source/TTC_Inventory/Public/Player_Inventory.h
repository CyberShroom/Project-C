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

UCLASS(meta = (ShortToolTip = "UI Inventory for the player."))
class TTC_INVENTORY_API UPlayer_Inventory : public UUIInventory
{
	GENERATED_BODY()

private:
	/// <summary>
	/// The number of slots per inventory row.
	/// </summary>
	UPROPERTY()
	uint8 slotsPerRow = 4;

protected:
	/// <summary>
	/// Fills the item_slot list with item_slots. Stops at currentSize or maxSize if currentSize exceeds it.
	/// </summary>
	virtual void FillList(UWidget* parent) override;

public:
	/// <summary>
	/// The number of rows in the inventory
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The number of rows in the inventory."))
	uint8 currentRows = 0;
};

//////////////////////////////////////////////////////////
// Create an inventory object							//
// Create a ui inventory object							//
// Both objects need to know the existence of the other	//
// Inventory is stored in player controller				//
// UI_Inventory is stored in the inventory ui			//
//////////////////////////////////////////////////////////
