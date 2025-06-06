// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SDIO_Inventory_Enums.h"
#include "Components/Image.h"
#include "SDIO_Item.h"
#include "Components/SizeBox.h"
#include "Item_Slot.generated.h"

UCLASS(meta = (ShortToolTip = "Base class for ui inventory slots. Contains common properties and functions."))
class SDIO_INVENTORY_API UItem_Slot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	UImage* Sprite;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	USizeBox* SizeBox;

	/// <summary>
	/// The item contained within this item_slot
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The item contained within this slot."))
	USDIO_Item* containedItem = nullptr;

	/// <summary>
	/// Sets the sprite image of the child class.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Sets the image sprite."))
	void SetSprite();

public:
	/// <summary>
	/// The type of item accepted by this slot
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "Defines what kind of items can be stored in this slot."))
	ESlotType type = ESlotType::Universal;

	/// <summary>
	/// Returns the UImage of this slot.
	/// </summary>
	UFUNCTION(BlueprintPure, Category = "SDIO_Inventory", meta = (Tooltip = "Returns the UImage object of this item slot."))
	UImage* GetSprite();

	/// <summary>
	/// Returns the item contained in this slot.
	/// </summary>
	UFUNCTION(BlueprintPure, Category = "SDIO_Inventory", meta = (Tooltip = "Returns the item contained in this slot."))
	USDIO_Item* GetContainedItem();

	/// <summary>
	/// Sets the item contained in this slot to the given item.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory", meta = (Tooltip = "Sets the item contained in this slot to the given item."))
	virtual void SetContainedItem(USDIO_Item* newItem);

	virtual void NativeConstruct() override;
};
