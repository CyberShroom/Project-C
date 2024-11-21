// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Player_Controller.h"


void ABase_Player_Controller::InitializeUIInventory()
{
	ABase_HUD* hudRef = Cast<ABase_HUD>(GetHUD());

	//Check if the hud is loaded properly
	if (IsValid(hudRef))
	{
		//Check if the uiManager has been set yet
		if (IsValid(hudRef->uiManagerRef))
		{
			USDIO_UIWindow_Inventory* invRef = Cast<USDIO_UIWindow_Inventory>(hudRef->uiManagerRef->Inventory_Window);
			AShip* shipRef = Cast<AShip>(GetPawn());
			
			//Check if the ui is loaded properly
			if (IsValid(invRef) && IsValid(shipRef))
			{
				invRef->InitializeAttributes(playerInventory, shipRef->shipInventory);
				invRef->onMoveItemToHotbar.AddUniqueDynamic(this, &ABase_Player_Controller::ServerRPC_MoveItemToHotbar);
				invRef->onMoveItemFromHotbar.AddUniqueDynamic(this, &ABase_Player_Controller::ServerRPC_MoveItemFromHotbar);
				return;
			}
		}
	}

	//If any of these are false, Run this again next frame.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ABase_Player_Controller::InitializeUIInventory);
}

void ABase_Player_Controller::Initialize()
{
	playerInventory = NewObject<UInventory>();

	if (GetPawn())
	{
		GetPawn()->SetActorLocation(FVector(0.0, 0.0, 0.0));
		playerShip = Cast<AShip>(GetPawn());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find a UShip that belongs to this player controller."));
	}

	playerInventory->Initialize(24, 8);
	InitializeUIInventory();
}

void ABase_Player_Controller::Security_AddItemToInventory(USDIO_Item* newItem)
{
	//Only Server may run this
	if (HasAuthority())
	{
		//Do not add the item if its the host. This will result in duplicates
		if (!IsLocalPlayerController())
		{
			playerInventory->AddItemToInventory(newItem);
		}
		ClientRPC_AddItemToInventory(newItem->instanceID);
	}
}

void ABase_Player_Controller::ClientRPC_AddItemToInventory_Implementation(FGuid itemID)
{
	UTTC_Item* newItem = NewObject<UTTC_Item>(GetWorld(),itemRef);
	newItem->instanceID = itemID;
	playerInventory->AddItemToInventory(newItem);
}

void ABase_Player_Controller::ServerRPC_MoveItemToHotbar_Implementation(FGuid itemID)
{
	if (playerInventory->GetItemFromInventory(itemID) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("An item with the GUID %s could not be found."), *itemID.ToString());
		return;
	}

	playerShip->shipInventory->AddItemToInventory(playerInventory->GetItemFromInventory(itemID));
	playerInventory->RemoveItemFromInventory(itemID);
}

void ABase_Player_Controller::ServerRPC_MoveItemFromHotbar_Implementation(FGuid itemID)
{
	if (playerShip->shipInventory->GetItemFromInventory(itemID) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("An item with the GUID %s could not be found."), *itemID.ToString());
		return;
	}

	playerInventory->AddItemToInventory(playerShip->shipInventory->GetItemFromInventory(itemID));
	playerShip->shipInventory->RemoveItemFromInventory(itemID);
}

void ABase_Player_Controller::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABase_Player_Controller, moveSpeed);
	DOREPLIFETIME(ABase_Player_Controller, turnSpeed);
}

