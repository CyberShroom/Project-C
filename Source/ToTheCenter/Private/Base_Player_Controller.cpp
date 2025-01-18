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
				invRef->onFinishInitialization.AddUniqueDynamic(this, &ABase_Player_Controller::SetMouseSlot);
				invRef->InitializeAttributes(playerInventory, shipRef->shipInventory);
				invRef->onMoveItemBetweenInventories.AddUniqueDynamic(this, &ABase_Player_Controller::DelegateInventoryInteractionHandler);
				return;
			}
		}
	}

	//If any of these are false, Run this again next frame.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ABase_Player_Controller::InitializeUIInventory);
}

void ABase_Player_Controller::DelegateInventoryInteractionHandler(FGuid itemID, EInventoryID targetID, EInventoryID originID)
{
	//If either inventory IDs are invalid, do not run this.
	if (targetID == EInventoryID::NOID)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller attempted to move items between inventories, but no valid TARGET inventory was given!"));
		return;
	}
	else if (originID == EInventoryID::NOID)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller attempted to move items between inventories, but no valid ORIGIN inventory was given!"));
		return;
	}

	//If is the host, run the server RPC instead
	if (HasAuthority() && IsLocalPlayerController())
	{
		ServerRPC_MoveItemBetweenInventories(itemID, targetID, originID);
		return;
	}

	//The item that will be moved
	USDIO_Item* itemToMove = nullptr;

	//Grab the item from the origin inventory. Remove it after its found. NOTE: RemoveItem function has built in nullptr detection, so its not needed here.
	switch (originID)
	{
		case EInventoryID::Main_Inventory:
			itemToMove = playerInventory->GetItemFromInventory(itemID);
			playerInventory->RemoveItemFromInventory(itemID);
			break;
		case EInventoryID::Hotbar_Inventory:
			itemToMove = playerShip->shipInventory->GetItemFromInventory(itemID);
			playerShip->shipInventory->RemoveItemFromInventory(itemID);
			break;
		case EInventoryID::Equipment_Inventory:
			break;
		case EInventoryID::Passive_Inventory:
			break;
	}

	//If no item is found, don't try to add it.
	if (itemToMove == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller could not find An item with the GUID %s while trying to move items between inventories."), *itemID.ToString());
		return;
	}

	//Add the item to the target inventory
	switch (targetID)
	{
		case EInventoryID::Main_Inventory:
			playerInventory->AddItemToInventory(itemToMove, false);
			break;
		case EInventoryID::Hotbar_Inventory:
			playerShip->shipInventory->AddItemToInventory(itemToMove, false);
			break;
		case EInventoryID::Equipment_Inventory:
			break;
		case EInventoryID::Passive_Inventory:
			break;
	}

	//Reflect this on the server's side
	ServerRPC_MoveItemBetweenInventories(itemID, targetID, originID);
}

void ABase_Player_Controller::SetMouseSlot(UItem_Slot* mouseRef)
{
	mouseSlot = mouseRef;
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

	playerInventory->Initialize(24, 8, EInventoryID::Main_Inventory);
	InitializeUIInventory();
}

void ABase_Player_Controller::AdvancedAddItemToInventory(USDIO_Item* newItem, bool bIsPickup)
{
	//Do not add the item if its the host. This will result in duplicates
	playerInventory->AddItemToInventory(newItem, bIsPickup);
	if (!IsLocalPlayerController() && bIsPickup)
	{
		ClientRPC_AddItemToInventory(newItem->instanceID, bIsPickup);
	}
}

void ABase_Player_Controller::ClientRPC_AddItemToInventory_Implementation(FGuid itemID, bool bIsPickup)
{
	UTTC_Item* newItem = NewObject<UTTC_Item>(GetWorld(),itemRef);
	newItem->instanceID = itemID;
	playerInventory->AddItemToInventory(newItem, bIsPickup);
}

void ABase_Player_Controller::ServerRPC_MoveItemBetweenInventories_Implementation(FGuid itemID, EInventoryID targetID, EInventoryID originID)
{
	//If either inventory IDs are invalid, do not run this.
	if (targetID == EInventoryID::NOID)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller attempted to move items between inventories, but no valid TARGET inventory was given!"));
		return;
	}
	else if(originID == EInventoryID::NOID)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller attempted to move items between inventories, but no valid ORIGIN inventory was given!"));
		return;
	}

	//The item that will be moved
	USDIO_Item* itemToMove = nullptr;

	//Grab the item from the origin inventory. Remove it after its found. NOTE: RemoveItem function has built in nullptr detection, so its not needed here.
	switch (originID)
	{
		case EInventoryID::Main_Inventory:
			itemToMove = playerInventory->GetItemFromInventory(itemID);
			playerInventory->RemoveItemFromInventory(itemID);
			break;
		case EInventoryID::Hotbar_Inventory:
			itemToMove = playerShip->shipInventory->GetItemFromInventory(itemID);
			playerShip->shipInventory->RemoveItemFromInventory(itemID);
			break;
		case EInventoryID::Equipment_Inventory:
			break;
		case EInventoryID::Passive_Inventory:
			break;
	}

	//If no item is found, don't try to add it.
	if (itemToMove == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Controller could not find An item with the GUID %s while trying to move items between inventories."), *itemID.ToString());
		return;
	}

	//Add the item to the target inventory
	switch (targetID)
	{
		case EInventoryID::Main_Inventory:
			playerInventory->AddItemToInventory(itemToMove, false);
			break;
		case EInventoryID::Hotbar_Inventory:
			playerShip->shipInventory->AddItemToInventory(itemToMove, false);
			break;
		case EInventoryID::Equipment_Inventory:
			break;
		case EInventoryID::Passive_Inventory:
			break;
	}
}

void ABase_Player_Controller::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABase_Player_Controller, moveSpeed);
	DOREPLIFETIME(ABase_Player_Controller, turnSpeed);
}

