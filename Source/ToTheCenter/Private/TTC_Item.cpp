// Fill out your copyright notice in the Description page of Project Settings.


#include "TTC_Item.h"

FGuid UTTC_Item::getID()
{
	return itemID;
}

FString UTTC_Item::GetInstance()
{
	return instanceID;
}

UTexture2D* UTTC_Item::GetSprite()
{
	return sprite;
}

void UTTC_Item::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTTC_Item, name);
	DOREPLIFETIME(UTTC_Item, sprite);
	DOREPLIFETIME(UTTC_Item, itemID);
}
