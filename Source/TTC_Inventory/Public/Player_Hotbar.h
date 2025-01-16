// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIInventory.h"
#include "Button_Item_Slot.h"
#include "Player_Hotbar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryInteraction2, UButton_Item_Slot*, slot, EInventoryID, id);

UCLASS()
class TTC_INVENTORY_API UPlayer_Hotbar : public UUIInventory
{
	GENERATED_BODY()
	
private:
	/// <summary>
	/// Handles logic related to clicking a button in the inventory.
	/// </summary>
	UFUNCTION()
	void OnClickHandler(UButton_Item_Slot* clickedSlot);

protected:
	/// <summary>
	/// Fills the item_slot list with item_slots. Stops at currentSize or maxSize if currentSize exceeds it.
	/// </summary>
	virtual void FillList(UWidget* parent) override;

public:
	/// <summary>
	/// Event Signature. Called when an inventory or hotbar button is clicked and no item is contained in the mouse.
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "TTC_Inventory | Events", meta = (Tooltip = "An event that, when called, checks if a swap has occurred."))
	FInventoryInteraction2 InventoryInteractionEvent;

	/// <summary>
	/// The item contained in the mouse.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The item contained in the mouse."))
	USDIO_Item* mouseItem = nullptr;

	/// <summary>
	/// Handles logic on how the inventory is sorted.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "TTC_Inventory", meta = (Tooltip = "Handles logic on how the inventory is sorted."))
	void HandleInventoryManagement(UButton_Item_Slot* clickedSlot, USDIO_Item* newItem);
};
