// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Slot.h"
#include "Components/Button.h"
#include "Button_Item_Slot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryButtonClicked, UButton_Item_Slot*, slot);

UCLASS(meta = (ShortToolTip = "Child of the Item_Slot with button and sprite functionality."))
class SDIO_INVENTORY_API UButton_Item_Slot : public UItem_Slot
{
	GENERATED_BODY()

private:
	/// <summary>
	/// Broadcasts OnButtonClicked when called
	/// </summary>
	UFUNCTION()
	void DelegateButtonClicked();
	
protected:
	//Object References//
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	UButton* Button;

public:
	/// <summary>
	/// Event Signature. Meant to be called when the button is clicked.
	/// </summary>
	UPROPERTY(BlueprintAssignable, Category = "SDIO_Inventory | Events", meta = (Tooltip = "An event that, when called, attempts to move items between inventory slots."))
	FInventoryButtonClicked OnButtonClicked;

	virtual void NativeConstruct() override;
};
