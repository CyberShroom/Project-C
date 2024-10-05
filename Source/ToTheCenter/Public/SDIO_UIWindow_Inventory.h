// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Window.h"
#include "Inventory_Slot.h"
#include "Base_Player_Controller.h"
#include "SDIO_UIWindow_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API USDIO_UIWindow_Inventory : public UUI_Window
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TSubclassOf<UUserWidget> subWidget;

	UPROPERTY()
	ABase_Player_Controller* owningController;

	UFUNCTION()
	void MoveInventoryItems(UInventory_Slot* clickedSlot);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HierarchyReference", meta = (bindWidget))
	UWidget* Hotbar;

	UPROPERTY(BlueprintReadOnly, Category = "HierarchyReference", meta = (bindWidget))
	UWidget* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	TArray<UInventory_Slot*> inventorySlotList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	TArray<UInventory_Slot*> hotbarSlotList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information")
	UTTC_Item* mouseItem;

public:
	UFUNCTION(BlueprintCallable, Category = "SDIO_UI")
	void SetInventorySize(uint8 size);

	UFUNCTION(BlueprintCallable, Category = "SDIO_UI")
	void SetHotbarSize(uint8 size);

	virtual void NativeConstruct() override;
};
