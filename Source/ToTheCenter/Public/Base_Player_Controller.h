// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Ship.h"
#include "Net/UnrealNetwork.h"
#include "UI_Window.h"
#include "Inventory.h"
#include "Base_HUD.h"
#include "SDIO_UIWindow_Inventory.h"
#include "TTC_Item.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Base_Player_Controller.generated.h"

class USDIO_UIWindow_Inventory; //Forward declaration

UCLASS()
class TOTHECENTER_API ABase_Player_Controller : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	USDIO_UIWindow_Inventory* inventoryUIReference; //Forward declaration plz ignore

	UPROPERTY()
	UAudioComponent* hullDamageAudioComponent;

	UPROPERTY()
	UAudioComponent* shieldDamageAudioComponent;

	UPROPERTY()
	UAudioComponent* armorDamageAudioComponent;

	UPROPERTY()
	UAudioComponent* shieldBreakAudioComponent;

	UPROPERTY()
	UAudioComponent* armorBreakAudioComponent;

	/// <summary>
	/// Attempts to run the initializer for the UI Inventory.
	/// </summary>
	UFUNCTION()
	void InitializeUIInventory();

	/// <summary>
	/// Handles the inventory interaction event
	/// </summary>
	UFUNCTION()
	void DelegateInventoryInteractionHandler(FGuid itemID, EInventoryID targetID, EInventoryID originID);

	/// <summary>
	/// Sets mouseSlot.
	/// </summary>
	UFUNCTION()
	void SetMouseSlot(UItem_Slot* mouseRef);

	UFUNCTION()
	void InitializeHealthSystem(USDIO_UIWindow_Inventory* invRef, AShip* shipRef);
	
	/// <summary>
	/// Plays the damage sound cue when the hull takes damage
	/// </summary>
	UFUNCTION()
	void PlayHullDamageSoundCue(float currentHull, float damage);

	/// <summary>
	/// Plays the damage sound cue when the shield takes damage
	/// </summary>
	UFUNCTION()
	void PlayShieldDamageSoundCue(float currentShield, float damage);

	/// <summary>
	/// Plays the damage sound cue when the armor takes damage
	/// </summary>
	UFUNCTION()
	void PlayArmorDamageSoundCue(float currentArmor, float damage);

	/// <summary>
	/// Plays the break sound cue when the shield breaks
	/// </summary>
	UFUNCTION()
	void PlayShieldBreakSoundCue();

	/// <summary>
	/// Plays the break sound cue when the armor breaks
	/// </summary>
	UFUNCTION()
	void PlayArmorBreakSoundCue();

protected:
	/// <summary>
	/// Mouse slot for the inventory.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "Mouse slot for the inventory."))
	UItem_Slot* mouseSlot;

	/// <summary>Reference to the AShip subclass of the pawn.</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "Reference to the player pawn."))
	AShip* playerShip;

	/// <summary>
	/// Sound that plays when the connected pawn takes damage.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "Sound that plays when the connected pawn takes damage."))
	USoundCue* hullDamageSound;

	/// <summary>
	/// Sound that plays when the connected pawn takes damage.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "Sound that plays when the connected pawn takes damage."))
	USoundCue* shieldDamageSound;

	/// <summary>
	/// Sound that plays when the connected pawn takes damage.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "Sound that plays when the connected pawn takes damage."))
	USoundCue* armorDamageSound;

	/// <summary>
	/// Sound that plays when the connected pawns shield breaks.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "Sound that plays when the connected pawns shield breaks."))
	USoundCue* shieldBreakSound;

	/// <summary>
	/// Sound that plays when the connected pawns armor breaks.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "Sound that plays when the connected pawns armor breaks."))
	USoundCue* armorBreakSound;

	/// <summary>Initializes the player controller.</summary>
	UFUNCTION(BlueprintCallable, Category = "TTC | Initializers", meta = (Tooltip = "Initializes the base player controller. This must be ran in BeginPlay BP!"))
	void Initialize();

public:
	/// <summary>
	/// The players logical inventory
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The players logical inventory"))
	UInventory* playerInventory;

	/// <summary>
	/// Adds an item to the players inventory. Can only be called from the server.
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "TTC", meta = (Tooltip = "Adds an item to the players inventory."))
	void AdvancedAddItemToInventory(USDIO_Item* newItem, bool bIsPickup);

	/// <summary>
	/// Tells the client to create an item of the given name with the given id.
	/// </summary>
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "TTC | ClientRPC", meta = (Tooltip = "Tells the client to create an item of the given name with the given id."))
	void ClientRPC_AddItemToInventory(FGuid itemID, bool bIsPickup);

	/// <summary>
	/// Tells the server to move an item from the origin to the target
	/// </summary>
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "TTC | ServerRPC", meta = (Tooltip = "Tells the server to move an item from the inventory to the hotbar."))
	void ServerRPC_MoveItemBetweenInventories(FGuid itemID, EInventoryID targetID, EInventoryID originID);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTTC_Item> itemRef;

	ABase_Player_Controller();
};
