// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIInventory.h"
#include "Button_Item_Slot.h"
#include "Player_Hotbar.generated.h"

UCLASS()
class TTC_INVENTORY_API UPlayer_Hotbar : public UUIInventory
{
	GENERATED_BODY()
	
protected:
	/// <summary>
	/// Fills the item_slot list with item_slots. Stops at currentSize or maxSize if currentSize exceeds it.
	/// </summary>
	virtual void FillList(UWidget* parent) override;
};
