// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Window.h"
#include "Inventory_Slot.h"
#include "Base_Player_Controller.h"
#include "Components/UniformGridPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "SDIO_UIWindow_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API USDIO_UIWindow_Inventory : public UUI_Window
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void MoveInventoryItems(UInventory_Slot* clickedSlot);

	UFUNCTION()
	void SwapInventoryItems(UInventory_Slot* clickedSlot, UTTC_Item* slotItem, UTTC_Item* newItem);

	UPROPERTY()
	uint8 slotsPerRow = 4;

	UPROPERTY()
	uint8 maxInventorySize = 20;

	UPROPERTY()
	uint8 maxHotbarSize = 4;

	UPROPERTY()
	uint8 currentInventorySize = 8;

	UPROPERTY()
	uint8 currentHotbarSize = 2;

	UPROPERTY()
	ABase_Player_Controller* owningController;

	UFUNCTION()
	void SetController();

	UFUNCTION()
	void SetInventoryPanel();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Reference to the inventory slot for inventory creation."))
	TSubclassOf<UInventory_Slot> subWidget;

	UPROPERTY(BlueprintReadOnly, Category = "HierarchyReference", meta = (bindWidget))
	UUniformGridPanel* Hotbar;

	UPROPERTY(BlueprintReadOnly, Category = "HierarchyReference", meta = (bindWidget))
	UUniformGridPanel* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	TArray<UInventory_Slot*> inventorySlotList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	TArray<UInventory_Slot*> hotbarSlotList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information")
	UTTC_Item* mouseItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information")
	UCanvasPanelSlot* inventoryPanelSlotReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information")
	FMargin slotMargin;

public:
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI")
	void SetInventorySize();

	UFUNCTION(BlueprintCallable, Category = "SDIO_UI")
	void SetHotbarSize();

	UFUNCTION(BlueprintCallable)
	void AddItemToUIInventory(UTTC_Item* newItem);

	virtual void NativeConstruct() override;
};
