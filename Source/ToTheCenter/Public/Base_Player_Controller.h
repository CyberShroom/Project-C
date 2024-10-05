// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ship.h"
#include "Net/UnrealNetwork.h"
#include "Base_Player_Controller.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API ABase_Player_Controller : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	uint8 maxInventorySize = 24;

	/// <summary>Reference to the AShip subclass of the pawn.</summary>
	UPROPERTY()
	AShip* playerShip;

protected:
	/// <summary>Initializes the player controller.</summary>
	UFUNCTION(BlueprintCallable, Category = "Initializers", meta = (Tooltip = "Initializes the base player controller. This must be ran in BeginPlay BP!"))
	void Initialize();

	/// <summary>An array of the players inventory.</summary>
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "An array of the player's inventory. DO NOT EDIT! USE SET FUNCTION INSTEAD!"))
	TArray<UTTC_Item*> inventoryList;

	/// <summary>The ships move speed</summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships move speed."))
	float moveSpeed = 100.0;

	/// <summary>The ships turn speed</summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships turn speed."))
	float turnSpeed = 100.0;

	//UFUNCTION(Client, Reliable, BlueprintCallable, Category = "SDIO_Player | Client RPC", meta = (Tooltip = "Tells the client to add an item to its inventory."))
	//void ClientRPC_AddItemToClientInventory(UTTC_Item* newItem);

public:
	UFUNCTION(BlueprintCallable, Category = "SDIO_Player", meta = (Tooltip = "Adds an item to the players inventory."))
	void AddItemToInventory(UTTC_Item* newItem);

	UFUNCTION(BlueprintPure, Category = "SDIO_Player")
	uint8 GetMaxInventorySize() const;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
};
