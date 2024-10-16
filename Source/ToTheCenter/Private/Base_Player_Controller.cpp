// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Player_Controller.h"

//void ABase_Player_Controller::ClientRPC_AddItemToClientInventory(UTTC_Item* newItem)
//{
//
//}

void ABase_Player_Controller::Initialize()
{
	if (GetPawn())
	{
		GetPawn()->SetActorLocation(FVector(0.0, 0.0, 0.0));
		playerShip = Cast<AShip>(GetPawn());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find a UShip that belongs to this player controller."));
	}
}

void ABase_Player_Controller::AddItemToInventory(UTTC_Item* newItem)
{
	UE_LOG(LogTemp, Warning, TEXT("Attempting to add item."));
	if (HasAuthority())
	{
		if (inventoryList.Num() < maxInventorySize && inventoryList.Num() < currentInventorySize)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item added. Attempting CAll!"));
			inventoryList.Add(newItem);
			OnPickupItem.Broadcast(newItem);
		}
	}
}

uint8 ABase_Player_Controller::GetMaxInventorySize() const
{
	return maxInventorySize;
}

void ABase_Player_Controller::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABase_Player_Controller, moveSpeed);
	DOREPLIFETIME(ABase_Player_Controller, turnSpeed);
	DOREPLIFETIME(ABase_Player_Controller, inventoryList);
	DOREPLIFETIME(ABase_Player_Controller, currentInventorySize);
}

