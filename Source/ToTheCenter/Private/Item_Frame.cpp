// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Frame.h"

// Sets default values
AItem_Frame::AItem_Frame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(scene);

	widgetRef = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemSlotWidget"));
	widgetRef->SetupAttachment(scene);

	interactWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("interactWidget"));
	interactWidget->SetupAttachment(scene);
}

void AItem_Frame::Multicast_SetItem_Implementation(const FString& item)
{
	//Retry until the slot reference is set
	if (!IsValid(slotRef))
	{
		UE_LOG(LogTemp, Error, TEXT("FAIL"));
		return;
	}

	if (item.Equals("Dev_Item"))
	{
		UTTC_Item* newItem = NewObject<UTTC_Item>(this, tempItem);
		newItem->Initialize();
		slotRef->SetContainedItem(newItem);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("An item with an id of %s could not be found. Item Frame cannot set item."), *item);
	}
}

// Called when the game starts or when spawned
void AItem_Frame::BeginPlay()
{
	Super::BeginPlay();

	//Set slot Reference
	slotRef = Cast<UItem_Slot>(widgetRef->GetWidget());
}

// Called every frame
void AItem_Frame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IInteractable_Interface::CheckProx(GetActorLocation(), interactWidget, GetWorld());
}

void AItem_Frame::Interact_Implementation(APlayerController* controller)
{
	ABase_Player_Controller* player = Cast<ABase_Player_Controller>(controller);

	//Add the item to the players inventory
	player->AdvancedAddItemToInventory(slotRef->GetContainedItem(), true);

	//Destroy this
	Destroy();
}

