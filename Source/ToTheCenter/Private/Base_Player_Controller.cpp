// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_Player_Controller.h"

//void ABase_Player_Controller::ClientRPC_AddItemToClientInventory(UTTC_Item* newItem)
//{
//
//}

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
			
			//Check if the ui is loaded properly
			if (IsValid(invRef))
			{
				invRef->InitializeAttributes(playerInventory);
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
	if (HasAuthority())
	{
		playerInventory->AddItemToInventory(newItem);
	}
}

void ABase_Player_Controller::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABase_Player_Controller, moveSpeed);
	DOREPLIFETIME(ABase_Player_Controller, turnSpeed);
}

