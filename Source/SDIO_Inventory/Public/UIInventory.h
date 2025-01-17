// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "SDIO_Item.h"
#include "Item_Slot.h"
#include "Components/UniformGridPanel.h"
#include "Inventory.h"
#include "Button_Item_Slot.h"
#include "UIInventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryInteraction, UButton_Item_Slot*, slot, EInventoryID, id);

UCLASS(meta = (ShortToolTip = "UI Inventory object. Contains the ui inventory and all logic associated with it."))
class SDIO_INVENTORY_API UUIInventory : public UObject
{
	GENERATED_BODY()
private:
	/// <summary>
	/// Inventory ID
	/// </summary>
	UPROPERTY()
	EInventoryID inventoryID = EInventoryID::NOID;

protected:
	/// <summary>
	/// The ui inventory
	/// </summary>
	UPROPERTY()
	TArray<UItem_Slot*> slotList;

	/// <summary>
	/// Max size of the ui inventory
	/// </summary>
	UPROPERTY()
	uint8 maxSize = 0;

	/// <summary>
	/// Called at the end of initialization. Designed to be used to fill the list.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Fills the item_slot list until it reaches the current size. Designed to be overriden by subclasses."))
	virtual void FillList(UWidget* parent);

public:
	/// <summary>
	/// Event signature. Called when a button within the inventory is clicked by the user.
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "SDIO_Inventory | Events", meta = (Tooltip = "Called when the user clicks a button in the inventory."))
	FInventoryInteraction OnInventoryInteraction;
	/// <summary>
	/// Reference to the child item slot
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Reference to the widget to construct"))
	TSubclassOf<UItem_Slot> widgetRef;

	/// <summary>
	/// Current size of the inventory
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The current size of the ui inventory."))
	uint8 currentSize = 0;

	/// <summary>
	/// The item contained in the mouse.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The item contained in the mouse."))
	USDIO_Item* mouseItem = nullptr;

	/// <summary>
	/// UI Inventory object initializer
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory | Initializers", meta = (Tooltip = "Initializes the ui inventory object. This must be ran after creating the object."))
	void Initialize(uint8 maxInventorySize, uint8 initialInventorySize, UInventory* inventoryRef, UWidget* panelRef, TSubclassOf<UItem_Slot> widget, EInventoryID id);

	/// <summary>
	/// Adds the given item to an empty slot in the inventory containers
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Adds the given item to the inventory object."))
	void AddItemToUIInventory(USDIO_Item* newItem);

	/// <summary>
	/// Broadcasts the onInventoryInteraction event. Should be assigned to the onClick event of buttons.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Broadcasts the onInventoryInteraction event. Should be assigned to the onClick event of buttons."))
	void DelegateOnClickHandler(UButton_Item_Slot* clickedSlot);

	/// <summary>
	/// Handles logic on how the inventory is sorted.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "TTC_Inventory", meta = (Tooltip = "Handles logic on how the inventory is sorted."))
	void HandleInventoryManagement(UButton_Item_Slot* clickedSlot, USDIO_Item* newItem);

	/// <summary>
	/// Returns the inventory ID
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Returns the inventory ID"))
	EInventoryID GetInventoryID();
};

//////////////////////////////////////////////////////////
// Create an inventory object							//
// Create a ui inventory object							//
// Both objects need to know the existence of the other	//
// Inventory is stored in player controller				//
// UI_Inventory is stored in the inventory ui			//
//////////////////////////////////////////////////////////
