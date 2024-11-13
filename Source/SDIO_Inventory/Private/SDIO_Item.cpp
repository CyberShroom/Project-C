// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_Item.h"

void USDIO_Item::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USDIO_Item, name);
	DOREPLIFETIME(USDIO_Item, sprite);
	DOREPLIFETIME(USDIO_Item, itemID);
	DOREPLIFETIME(USDIO_Item, instanceID);
}
