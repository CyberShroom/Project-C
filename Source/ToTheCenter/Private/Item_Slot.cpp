// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Slot.h"

void UItem_Slot::SetSprite(UImage* imageObject, UTexture2D* spriteObject)
{
	imageObject->SetBrushFromTexture(spriteObject);

	if (spriteObject)
	{
		imageObject->SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		imageObject->SetColorAndOpacity(FLinearColor(1.0,1.0,1.0,0.0));
	}
}

bool UItem_Slot::GetContainedItem(UTTC_Item*& existingItem)
{
	if (containedItem)
	{
		existingItem = containedItem;
		return true;
	}

	return false;
}

void UItem_Slot::SetContainedItem(UTTC_Item* newItem)
{
	//if(type)
}
