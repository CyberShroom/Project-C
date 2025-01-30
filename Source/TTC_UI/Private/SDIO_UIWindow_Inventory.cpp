// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UIWindow_Inventory.h"

void USDIO_UIWindow_Inventory::DelegateInventoryInteraction(UButton_Item_Slot* clickedSlot, EInventoryID id)
{
	//If the inventory is minimized. DO NOTHING!
	if (bIsMinimized)
	{
		return;
	}

	// Flag that determines if id should be the next last inventory interaction at end of function
	bool bUpdateInteraction = false;

	//If clickSlot contains an item, thats an interaction. Set flag to true.
	if (clickedSlot->GetContainedItem())
	{
		bUpdateInteraction = true;
	}

	//If the mouse slot has no item or, the last id and current id are the same, handle from inventory
	if (mouseSlot->GetContainedItem() == nullptr || lastInteractedInventory == id)
	{
		ReturnInventoryInteraction(clickedSlot, nullptr, id);
	}
	else
	{
		FGuid instanceID = mouseSlot->GetContainedItem()->instanceID;
		//Do different things based on the last interacted inventory. Remember that clicking an empty slot does not count as an interaction.
		switch (lastInteractedInventory)
		{
			case EInventoryID::NOID: //No interacted inventory, handle from inventory.
				ReturnInventoryInteraction(clickedSlot, nullptr, id);
				break;
			case EInventoryID::Main_Inventory:
				//Since lastinteractedinventory only updates on clicking a slot with an item in it, a swap has to occur.
				ReturnInventoryInteraction(clickedSlot, mouseSlot->GetContainedItem(), id);
				onMoveItemBetweenInventories.Broadcast(instanceID, id, lastInteractedInventory);
				break;
			case EInventoryID::Hotbar_Inventory:
				//Since lastinteractedinventory only updates on clicking a slot with an item in it, a swap has to occur.
				ReturnInventoryInteraction(clickedSlot, mouseSlot->GetContainedItem(), id); // swaps items
				onMoveItemBetweenInventories.Broadcast(instanceID, id, lastInteractedInventory); 
				break;
			case EInventoryID::Equipment_Inventory:
				//If id is equal
					//HandleInventory
				//Else
					//Swap has occurred
				break;
			case EInventoryID::Passive_Inventory:
				//ignore for now
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("Inventory Interaction Event From UIWindow_Inventory failed to recieve a correct ID."));
				break;
		}
	}

	//If flag is true, update last interaction
	if (bUpdateInteraction)
	{
		lastInteractedInventory = id;
	}
	else //Otherwise clear last interaction
	{
		lastInteractedInventory = EInventoryID::NOID;
	}
}

void USDIO_UIWindow_Inventory::ReturnInventoryInteraction(UButton_Item_Slot* clickedSlot, USDIO_Item* swapItem, EInventoryID id)
{
	//Runs the inventory handler for the given id and only the given id
	switch (id)
	{
		case EInventoryID::Main_Inventory:
			playerInventory->HandleInventoryManagement(clickedSlot, swapItem);
			break;
		case EInventoryID::Hotbar_Inventory:
			hotbarInventory->HandleInventoryManagement(clickedSlot, swapItem);
			break;
		case EInventoryID::Equipment_Inventory:
			break;
		case EInventoryID::Passive_Inventory:
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Return Inventory Interaction Event From UIWindow_Inventory failed to recieve a correct ID."));
			break;
	}
}

void USDIO_UIWindow_Inventory::SetInventoryPanelSize()
{
	if (playerInventory->currentRows != 0)
	{
		FVector2D panelSize = inventoryPanelSlotReference->GetSize();
		panelSize.Y = 52 * playerInventory->currentRows;
		inventoryPanelSlotReference->SetSize(panelSize);
	}
}

void USDIO_UIWindow_Inventory::InitializeAttributes(UInventory* invRef, UInventory* hotbarRef)
{
	//Attempt tp cast the Inventory panel
	UCanvasPanelSlot* panel = Cast<UCanvasPanelSlot>(Inventory->Slot);

	//Check if it succeeded
	if (IsValid(panel))
	{
		//On Success

		//Instantiate the Mouse Slot
		mouseSlot = CreateWidget<UItem_Slot>(this, mouseWidget);
		Canvas->AddChildToCanvas(mouseSlot);
		Cast<UCanvasPanelSlot>(mouseSlot->Slot)->SetZOrder(2);

		//Set the panelslot reference and the slotMargin reference
		inventoryPanelSlotReference = panel;

		//Initialize the player inventory
		playerInventory->Initialize(24, invRef->currentSize, invRef, Inventory, subWidget, EInventoryID::Main_Inventory, mouseSlot);
		playerInventory->OnInventoryInteraction.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::DelegateInventoryInteraction);

		//Initialize the player hotbar
		hotbarInventory->Initialize(8, hotbarRef->currentSize, hotbarRef, Hotbar, subWidget, EInventoryID::Hotbar_Inventory, mouseSlot);
		hotbarInventory->OnInventoryInteraction.AddUniqueDynamic(this, &USDIO_UIWindow_Inventory::DelegateInventoryInteraction);

		//Set the inventory panel size (Initialize contains the code for filling the inventory so this should run afterwards)
		SetInventoryPanelSize();

		//Initialzie top left bars
		HealthBar->InitializeAttributes(FLinearColor::Red, FLinearColor::Green, 0, 0);
		ShieldBar->InitializeAttributes(FLinearColor::Blue, FLinearColor(0.0f, 1.0f, 1.0f, 1.0f), 0, 0);

		onFinishInitialization.Broadcast(mouseSlot);
	}
	else
	{
		//If not, try again next frame
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, invRef, hotbarRef]() {InitializeAttributes(invRef, hotbarRef); });
	}
}

void USDIO_UIWindow_Inventory::ToggleInventoryState(bool isMinimized)
{
	bIsMinimized = isMinimized;
	FVector2D hotbarTranslation = Hotbar->GetRenderTransform().Translation;

	//tp hotbar immediatly when false
	if (bIsMinimized == false)
	{
		hotbarTranslation.X = 0;
		Hotbar->SetRenderTranslation(hotbarTranslation);

		if (mouseSlot->GetContainedItem())
		{
			mouseSlot->GetSprite()->SetOpacity(100);
		}
	}

	//Get information on the inventory panel since its dynamically sized
	float sizeY = inventoryPanelSlotReference->GetSize().Y + 10;
	FVector2D currentLocation = Inventory->GetRenderTransform().Translation;

	//Move the panel based on the toggle
	if (isMinimized)
	{
		currentLocation.Y += toggleSpeed;
	}
	else
	{
		currentLocation.Y -= toggleSpeed;
	}

	//Set the new translation
	Inventory->SetRenderTranslation(currentLocation);

	//Continue running until the translation is finished
	if (bIsMinimized == true && sizeY >= currentLocation.Y)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, isMinimized]() {ToggleInventoryState(bIsMinimized); });
		return;
	}
	else if (bIsMinimized == false && currentLocation.Y > 0)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, isMinimized]() {ToggleInventoryState(bIsMinimized); });
		return;
	}

	//tp hotbar when animation is done
	if (bIsMinimized)
	{
		hotbarTranslation.X = 300;
		Hotbar->SetRenderTranslation(hotbarTranslation);

		mouseSlot->GetSprite()->SetOpacity(0);
	}
}

void USDIO_UIWindow_Inventory::UpdateHullValue(float newValue, float amountChanged)
{
	HealthBar->SetLeftVarValue(newValue);
	HealthBar->UpdateText();
}

void USDIO_UIWindow_Inventory::UpdateMaxHullValue(float newValue)
{
	HealthBar->SetRightVarValue(newValue);
}

void USDIO_UIWindow_Inventory::UpdateShieldValue(float newValue, float amountChanged)
{
	ShieldBar->SetLeftVarValue(newValue);
	ShieldBar->UpdateText();
}

void USDIO_UIWindow_Inventory::UpdateMaxShieldValue(float newValue)
{
	ShieldBar->SetRightVarValue(newValue);
}

void USDIO_UIWindow_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	playerInventory = NewObject<UPlayer_Inventory>();
	hotbarInventory = NewObject<UPlayer_Hotbar>();
}
