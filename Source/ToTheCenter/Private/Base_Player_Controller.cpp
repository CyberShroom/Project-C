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

	playerInventory->Initialize(24, 8);
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

