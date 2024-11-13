// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ship.h"
#include "Net/UnrealNetwork.h"
#include "UI_Window.h"
#include "Inventory.h"
#include "Base_Player_Controller.generated.h"

class USDIO_UIWindow_Inventory; //Forward declaration

UCLASS()
class TOTHECENTER_API ABase_Player_Controller : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USDIO_UIWindow_Inventory* inventoryUIReference; //Forward declaration plz ignore

protected:
	/// <summary>Initializes the player controller.</summary>
	UFUNCTION(BlueprintCallable, Category = "Initializers", meta = (Tooltip = "Initializes the base player controller. This must be ran in BeginPlay BP!"))
	void Initialize();

	/// <summary>Reference to the AShip subclass of the pawn.</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "Reference to the player pawn."))
	AShip* playerShip;

	//UFUNCTION(Client, Reliable, BlueprintCallable, Category = "SDIO_Player | Client RPC", meta = (Tooltip = "Tells the client to add an item to its inventory."))
	//void ClientRPC_AddItemToClientInventory(UTTC_Item* newItem);

public:
	/// <summary>
	/// The players logical inventory
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The players logical inventory"))
	UInventory* playerInventory = NewObject<UInventory>();

	/// <summary>The ships move speed</summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships move speed."))
	float moveSpeed = 100.0;

	/// <summary>The ships turn speed</summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships turn speed."))
	float turnSpeed = 100.0;

	UFUNCTION(BlueprintCallable, Category = "SDIO_Player", meta = (Tooltip = "Adds an item to the players inventory."))
	void Security_AddItemToInventory(USDIO_Item* newItem);

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
};
