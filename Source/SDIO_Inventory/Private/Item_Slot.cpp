#include "Item_Slot.h"
// Fill out your copyright notice in the Description page of Project Settings.

USDIO_Item* UItem_Slot::GetContainedItem()
{
	return containedItem;
}

void UItem_Slot::SetContainedItem(USDIO_Item* newItem)
{
	containedItem = newItem;
	SetSprite();
}

UImage* UItem_Slot::GetSprite()
{
	return Sprite;
}

void UItem_Slot::SetSprite()
{
	if (Sprite == nullptr) { return; }//Only run this function if a UImage exists

	//if there is a contained item set the brush to the sprite
	if (containedItem)
	{
		//Set the brush to the material, otherwise set it to the texture
		if (IsValid(containedItem->material))
		{
			Sprite->SetBrushFromMaterial(containedItem->material);
			UE_LOG(LogTemp, Error, TEXT("Generate Material"));
		}
		else
		{
			Sprite->SetBrushFromTexture(containedItem->sprite);
			UE_LOG(LogTemp, Error, TEXT("Generate Texture"));
		}

		Sprite->SetColorAndOpacity(FLinearColor::White);
	}
	else //else remove the brush and make it invisible
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR"));
		Sprite->SetBrushFromTexture(nullptr);
		Sprite->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 0.0));
	}
}

void UItem_Slot::NativeConstruct()
{
	Super::NativeConstruct();
}
