// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Window.h"
#include "Button_Item_Slot.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Player_Inventory.h"
#include "Player_Hotbar.h"
#include "SDIO_UIWindow_Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveItemToHotbar, FGuid, itemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveItemFromHotbar, FGuid, itemID);

UCLASS(meta = (ShortToolTip = "The Inventory window base class"))
class TTC_UI_API USDIO_UIWindow_Inventory : public UUI_Window
{
	GENERATED_BODY()
	
private:
	/// <summary>
	/// The maximum number of slots the hotbar can have. The game does not support more than 4. Changing this value above 4 will not do anything.
	/// </summary>
	UPROPERTY()
	uint8 maxHotbarSize = 4;

	/// <summary>
	/// The current number of hotbar spaces
	/// </summary>
	UPROPERTY()
	uint8 currentHotbarSize = 2;

	/// <summary>
	/// Event Delegate. Checks if either the inventory or hotbar contain a mouse item. If they do, swap the item between inventories.
	/// </summary>
	UFUNCTION()
	void DelegateCheckForSwap(UButton_Item_Slot* clickedSlot);

protected:
	/// <summary>
	/// Contains a copy of the inventory slot to generate for the dynamic inventory.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Reference to the inventory slot for inventory creation."))
	TSubclassOf<UButton_Item_Slot> subWidget;

	//Hierarchy References//
	UPROPERTY(BlueprintReadOnly, Category = "HierarchyReference", meta = (bindWidget))
	UUniformGridPanel* Hotbar;

	UPROPERTY(BlueprintReadOnly, Category = "HierarchyReference", meta = (bindWidget))
	UUniformGridPanel* Inventory;

	/// <summary>
	/// A list of the inventory slots in the hotbar
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "A list of the inventory slots in the hotbar"))
	TArray<UButton_Item_Slot*> hotbarSlotList;

	/// <summary>
	/// The UCanvasPanelSlot reference on the inventory panel
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "Reference to the UCanvasPanelSlot of the inventory panel."))
	UCanvasPanelSlot* inventoryPanelSlotReference;

	/// <summary>
	/// The FMargin of the panel slots. Used to change the size of the panel in the dynamic inventory.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The FMargin of the panel slots. Used to change the size of the panel in the dynamic inventory."))
	FMargin slotMargin;

public:
	UPROPERTY(BlueprintAssignable, Category = "TTC_UI | Events", meta = (Tooltip = "Called when the user moves an item from the inventory to the hotbar."))
	FMoveItemToHotbar onMoveItemToHotbar;

	UPROPERTY(BlueprintAssignable, Category = "TTC_UI | Events", meta = (Tooltip = "Called when the user moves an item from the hotbar to the inventory."))
	FMoveItemToHotbar onMoveItemFromHotbar;

	/// <summary>
	/// The visual player inventory
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The visual player inventory."))
	UPlayer_Inventory* playerInventory;

	/// <summary>
	/// The visual player hotbar
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The visual player hotbar."))
	UPlayer_Hotbar* hotbarInventory;

	/// <summary>
	/// Sets the margins of the inventory panel.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "TTC_UI", meta = (Tooltip = "Sets the margins of the inventory panel."))
	void SetInventoryPanelSize();

	/// <summary>
	/// Initializes the inventory panel and inventory. Meant to be called from the invRef location.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "TTC_UI", meta = (Tooltip = "Initializes the inventory panel and inventory. Meant to be called from the invRef location."))
	void InitializeAttributes(UInventory* invRef, UInventory* hotbarRef);

	virtual void NativeConstruct() override;
};
