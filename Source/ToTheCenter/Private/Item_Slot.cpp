// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Slot.h"

void UItem_Slot::SetSprite(UImage* imageObject)
{
	if (containedItem)
	{
		imageObject->SetBrushFromTexture(containedItem->GetSprite());
		imageObject->SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		imageObject->SetBrushFromTexture(nullptr);
		imageObject->SetColorAndOpacity(FLinearColor(1.0,1.0,1.0,0.0));
	}
}

UTTC_Item* UItem_Slot::GetContainedItem()
{
	return containedItem;
}

void UItem_Slot::SetContainedItem(UTTC_Item* newItem, UImage* imageObject)
{
	containedItem = newItem;
	SetSprite(imageObject);
}
