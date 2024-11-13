// Fill out your copyright notice in the Description page of Project Settings.


#include "Button_Item_Slot.h"

void UButton_Item_Slot::DelegateButtonClicked()
{
	OnButtonClicked.Broadcast(this);
}

UImage* UButton_Item_Slot::GetSprite()
{
	return Sprite;
}

void UButton_Item_Slot::SetContainedItem(USDIO_Item* newItem)
{
	containedItem = newItem;
	SetSprite();
}

void UButton_Item_Slot::SetSprite()
{
	if (Sprite == nullptr) { return; }//Only run this function if a UImage exists

	//if there is a contained item set the brush to the sprite
	if (containedItem)
	{
		Sprite->SetBrushFromTexture(containedItem->sprite);
		Sprite->SetColorAndOpacity(FLinearColor::White);
	}
	else //else remove the brush and make it invisible
	{
		Sprite->SetBrushFromTexture(nullptr);
		Sprite->SetColorAndOpacity(FLinearColor(1.0, 1.0, 1.0, 0.0));
	}
}

void UButton_Item_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button && Sprite)
	{
		Button->OnClicked.AddUniqueDynamic(this, &UButton_Item_Slot::DelegateButtonClicked);
	}
}


