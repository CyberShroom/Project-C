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

				//Once finished, Initialize the player ui health system
				InitializeHealthSystem(invRef, shipRef);
				return;
			}
		}
	}

	//If any of these are false, Run this again next frame.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ABase_Player_Controller::InitializeUIInventory);
}

void ABase_Player_Controller::InitializePawn()
{
	if (GetPawn())
	{
		GetPawn()->SetActorLocation(FVector(0.0, 0.0, 0.0));
		playerShip = Cast<AShip>(GetPawn());
	}

	if (!IsValid(playerShip))
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ABase_Player_Controller::InitializePawn);
	}
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

void ABase_Player_Controller::InitializeHealthSystem(USDIO_UIWindow_Inventory* invRef, AShip* shipRef)
{
	//Setup hull events
	shipRef->onHullDamage.AddUniqueDynamic(invRef, &USDIO_UIWindow_Inventory::UpdateHullValue);
	shipRef->onHullHeal.AddUniqueDynamic(invRef, &USDIO_UIWindow_Inventory::UpdateHullValue);
	shipRef->onMaxHullChanged.AddUniqueDynamic(invRef, &USDIO_UIWindow_Inventory::UpdateMaxHullValue);

	//Setup shield events
	shipRef->onShieldDamage.AddUniqueDynamic(invRef, &USDIO_UIWindow_Inventory::UpdateShieldValue);
	shipRef->onMaxShieldChanged.AddUniqueDynamic(invRef, &USDIO_UIWindow_Inventory::UpdateMaxShieldValue);

	//Setup Armor events
	shipRef->onArmorDamage.AddUniqueDynamic(invRef, &USDIO_UIWindow_Inventory::UpdateArmorValue);
	shipRef->onGainArmor.AddUniqueDynamic(invRef, &USDIO_UIWindow_Inventory::UpdateArmorValue);

	//Setup Sound events, only if its local controller. I do not want to hear you take damage. Pls and Thx.
	if (IsLocalPlayerController())
	{
		shipRef->onHullDamage.AddUniqueDynamic(this, &ABase_Player_Controller::PlayHullDamageSoundCue);
		shipRef->onShieldDamage.AddUniqueDynamic(this, &ABase_Player_Controller::PlayShieldDamageSoundCue);
		shipRef->onArmorDamage.AddUniqueDynamic(this, &ABase_Player_Controller::PlayArmorDamageSoundCue);

		shipRef->onShieldBreak.AddUniqueDynamic(this, &ABase_Player_Controller::PlayShieldBreakSoundCue);
		shipRef->onArmorBreak.AddUniqueDynamic(this, &ABase_Player_Controller::PlayArmorBreakSoundCue);
	}

	invRef->UpdateMaxHullValue(shipRef->GetMaxHull());
	invRef->UpdateHullValue(shipRef->GetMaxHull(), 0);
	invRef->UpdateMaxShieldValue(shipRef->GetMaxShield());
	invRef->UpdateShieldValue(shipRef->GetMaxShield(), 0);
	invRef->UpdateArmorValue(shipRef->GetArmor(), 0);
}

void ABase_Player_Controller::PlayHullDamageSoundCue(float currentHull, float damage)
{
	if (damage <= 0)
	{
		return;
	}

	hullDamageAudioComponent->Play();
}

void ABase_Player_Controller::PlayShieldDamageSoundCue(float currentShield, float damage)
{
	if (damage <= 0 || currentShield - damage <= 0)
	{
		return;
	}

	shieldDamageAudioComponent->Play();
}

void ABase_Player_Controller::PlayArmorDamageSoundCue(float currentArmor, float damage)
{
	if (damage <= 0 || currentArmor - damage <= 0)
	{
		return;
	}

	armorDamageAudioComponent->Play();
}

void ABase_Player_Controller::PlayShieldBreakSoundCue()
{
	shieldBreakAudioComponent->Play();
}

void ABase_Player_Controller::PlayArmorBreakSoundCue()
{
	armorBreakAudioComponent->Play();
}

void ABase_Player_Controller::Initialize()
{
	//Create the inventory object
	playerInventory = NewObject<UInventory>();

	if (hullDamageSound && shieldDamageSound && armorDamageSound && shieldBreakSound && armorBreakSound)
	{
		//Set audio component sounds
		hullDamageAudioComponent->SetSound(hullDamageSound);
		shieldDamageAudioComponent->SetSound(shieldDamageSound);
		armorDamageAudioComponent->SetSound(armorDamageSound);
		shieldBreakAudioComponent->SetSound(shieldBreakSound);
		armorBreakAudioComponent->SetSound(armorBreakSound);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller failed to load audio. One or more Sound Cues are not set or failed to load!"));
	}
	

	//Set the player ship location
	InitializePawn();

	//Initialize the inventory and ui inventory
	playerInventory->Initialize(24, 8, EInventoryID::Main_Inventory);
	InitializeUIInventory();
}

void ABase_Player_Controller::ServerRPC_InputTurnStop_Implementation(FRotator predictedRotation)
{
	if (IsLocalController())
	{
		return;
	}

	playerShip->StopShipRotation(predictedRotation);
}

void ABase_Player_Controller::ServerRPC_InputMovementStop_Implementation(FVector predictedLocation)
{
	if (IsLocalController())
	{
		return;
	}

	//Prevents clients from exploiting movement
	movementTally++;
	UE_LOG(LogTemp, Error, TEXT("Tally is: %d"), movementTally);

	playerShip->StopShip(predictedLocation);
}

void ABase_Player_Controller::ServerRPC_InputTurn_Implementation(float joystickValue, FRotator predictedRotation)
{
	if (IsLocalController())
	{
		return;
	}

	playerShip->TurnShip(joystickValue, true, predictedRotation);
	playerShip->ClientRPC_CheckForRotationError(playerShip->GetTargetRotation(), predictedRotation);
}

void ABase_Player_Controller::ServerRPC_InputVertical_Implementation(float joystickValue, FVector predictedLocation, FVector clientForwardVector)
{
	if (IsLocalController() || movementTally > (1 / playerShip->GetMovementDuration()) * 2)
	{
		return;
	}

	//Prevents clients from exploiting movement
	movementTally++;
	UE_LOG(LogTemp, Error, TEXT("Tally is: %d"), movementTally);

	playerShip->MoveShip(joystickValue, true, clientForwardVector, predictedLocation);
	playerShip->ClientRPC_CheckForLocationError(playerShip->GetMovementTargetLocation(), predictedLocation);
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

void ABase_Player_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	tallyTimer += DeltaTime;

	if (tallyTimer >= 1)
	{
		movementTally = 0;
		tallyTimer = 0;
	}
}

ABase_Player_Controller::ABase_Player_Controller()
{
	//Create audio components
	hullDamageAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("hullAudio"));
	hullDamageAudioComponent->bAutoActivate = false; // Prevent it from playing on start
	hullDamageAudioComponent->SetupAttachment(RootComponent);

	shieldDamageAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("shieldAudio"));
	shieldDamageAudioComponent->bAutoActivate = false; // Prevent it from playing on start
	shieldDamageAudioComponent->SetupAttachment(RootComponent);

	armorDamageAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("armorAudio"));
	armorDamageAudioComponent->bAutoActivate = false; // Prevent it from playing on start
	armorDamageAudioComponent->SetupAttachment(RootComponent);

	shieldBreakAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("shieldBreakAudio"));
	shieldBreakAudioComponent->bAutoActivate = false; // Prevent it from playing on start
	shieldBreakAudioComponent->SetupAttachment(RootComponent);

	armorBreakAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("armorBreakAudio"));
	armorBreakAudioComponent->bAutoActivate = false; // Prevent it from playing on start
	armorBreakAudioComponent->SetupAttachment(RootComponent);
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

