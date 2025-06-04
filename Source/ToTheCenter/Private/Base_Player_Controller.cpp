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
	else
	{
		if (HasAuthority() && !IsLocalPlayerController())
		{
			playerShip->Initialize(false, true);
		}
		else
		{
			playerShip->Initialize(false, false);
		}
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
	shipRef->onShieldRegenTick.AddUniqueDynamic(invRef, &USDIO_UIWindow_Inventory::UpdateShieldValue);

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
	if (IsLocalPlayerController())
	{
		hullDamageAudioComponent->Play();
	}
}

void ABase_Player_Controller::PlayShieldDamageSoundCue(float currentShield, float damage)
{
	if (IsLocalPlayerController())
	{
		shieldDamageAudioComponent->Play();
	}
}

void ABase_Player_Controller::PlayArmorDamageSoundCue(float currentArmor, float damage)
{
	if (IsLocalPlayerController())
	{
		armorDamageAudioComponent->Play();
	}
}

void ABase_Player_Controller::PlayShieldBreakSoundCue()
{
	if (IsLocalPlayerController())
	{
		shieldBreakAudioComponent->Play();
	}
}

void ABase_Player_Controller::PlayArmorBreakSoundCue()
{
	if (IsLocalPlayerController())
	{
		armorBreakAudioComponent->Play();
	}
}

AActor* ABase_Player_Controller::SearchForClosestInteractable()
{
	//Collision variables
	TArray<FOverlapResult> overlaps;
	FCollisionShape sphere = FCollisionShape::MakeSphere(500);
	FCollisionQueryParams params;

	//Remove self
	params.AddIgnoredActor(this);

	//Grab all objects around the player
	GetWorld()->OverlapMultiByObjectType(
		overlaps,
		playerShip->GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllObjects),
		sphere,
		params
	);

	//actor to return
	AActor* closest = nullptr;
	float shortestDistance = 500;

	//Iterate over all found actors
	for (const FOverlapResult& e : overlaps)
	{
		AActor* actor = e.GetActor();
		
		//If the actor is not valid, ignore it
		if (!IsValid(actor)) continue;
		
		//Only count objects that are interactable
		if (actor->GetClass()->ImplementsInterface(UInteractable_Interface::StaticClass()))
		{
			//Get the distance from the player
			float distance = FVector::Dist(playerShip->GetActorLocation(), actor->GetActorLocation());

			//Make closest the actor with the least distance
			if (distance < shortestDistance)
			{
				closest = actor;
				shortestDistance = distance;
			}
		}
	}

	return closest;
}

void ABase_Player_Controller::ServerRPC_Interact_Implementation(AActor* target)
{
	if (!IsValid(target))
	{
		UE_LOG(LogTemp, Warning, TEXT("Interaction Event Target Invalid!"));
		return;
	}

	//Only allow if the object is within 500 units of the player on the server
	if (FVector::Dist(playerShip->GetActorLocation(), target->GetActorLocation()) <= 500)
	{
		IInteractable_Interface::Execute_Interact(target, this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server Rejected interaction event. %f"), FVector::Dist(playerShip->GetActorLocation(), target->GetActorLocation()));
		UE_LOG(LogTemp, Warning, TEXT("Server Rejected interaction event."));
	}
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

	playerShip->AddRotationToTimeline(predictedRotation, true);
}

void ABase_Player_Controller::ServerRPC_InputMovementStop_Implementation(FVector predictedLocation)
{
	//Prevent host from running this
	if (IsLocalController())
	{
		return;
	}

	playerShip->AddVectorToTimeline(predictedLocation, true);
}

void ABase_Player_Controller::ServerRPC_InputTurn_Implementation(float joystickValue)
{
	//Prevent host from running this
	if (IsLocalController())
	{
		return;
	}

	//Joystick value cannot be higher than 1.0 nor less than -1.0
	if (joystickValue > 1.0)
	{
		joystickValue = 1.0;
	}
	else if (joystickValue < -1.0)
	{
		joystickValue = -1.0;
	}

	//Add the rotation to the timeline
	playerShip->AddRotationToTimeline(playerShip->CalculateRotation(joystickValue));

	//playerShip->ClientRPC_CheckForRotationError(playerShip->GetTargetRotation(), predictedRotation);
}

void ABase_Player_Controller::ServerRPC_InputVertical_Implementation(FVector clientForwardVector, float joystickValue)
{
	//prevent host from running this
	if (IsLocalController())
	{
		return;
	}
	
	//Joystick value cannot be higher than 1.0 nor less than -1.0
	if (joystickValue > 1.0)
	{
		joystickValue = 1.0;
	}
	else if (joystickValue < -1.0)
	{
		joystickValue = -1.0;
	}

	//Add the forward vector to the timeline
	playerShip->AddVectorToTimeline(playerShip->CalculateMovementVector(joystickValue, clientForwardVector));
}

void ABase_Player_Controller::AdvancedAddItemToInventory(USDIO_Item* newItem, bool bIsPickup)
{
	//Do not add the item if its the host. This will result in duplicates
	playerInventory->AddItemToInventory(newItem, bIsPickup);
	if (!IsLocalPlayerController() && bIsPickup)
	{
		ClientRPC_AddItemToInventory(newItem->itemID, newItem->instanceID, bIsPickup);
	}
}

void ABase_Player_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ABase_Player_Controller::ClientRPC_AddItemToInventory_Implementation(const FString& itemName, FGuid itemID, bool bIsPickup)
{
	UTTC_Item* newItem = GetGameInstance()->GetSubsystem<UItemRegistry>()->GetItemFromID(itemName);

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

