// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Slot.h"
#include "Components/Button.h"
#include "Inventory_Slot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryButtonClicked, UInventory_Slot*, slot);

UCLASS(meta = (ShortToolTip = "An item slot used for the inventory and hotbar."))
class TOTHECENTER_API UInventory_Slot : public UItem_Slot
{
	GENERATED_BODY()

private:
	//UFUNCTION()
	//void updateSprite();

	UFUNCTION()
	void ButtonClicked();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	UButton* Button;
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	UImage* Sprite;

public:
	UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Event Dispatchers", meta = (Tooltip = "An event that, when called, attempts to move items between inventory slots."))
	FInventoryButtonClicked OnButtonClicked;

	UImage* GetSprite();

	virtual void NativeConstruct() override;
};
