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
		UE_LOG(LogTemp, Error, TEXT("Item Frame Slot Reference Is Not Set!"));
		return;
	}

	UTTC_Item* newItem = GetGameInstance()->GetSubsystem<UItemRegistry>()->GetItemFromID(item);
	slotRef->SetContainedItem(newItem);

	//Set widget size to keep things consistent
	if (newItem->sprite->GetSizeX() == newItem->sprite->GetSizeY())
	{
		widgetRef->SetDrawSize(FVector2D(32, 32));
	}
	else
	{
		widgetRef->SetDrawSize(FVector2D(64, 64));
	}
}

// Called when the game starts or when spawned
void AItem_Frame::BeginPlay()
{
	Super::BeginPlay();

	slotRef = Cast<UItem_Slot>(widgetRef->GetWidget());
}

bool AItem_Frame::SlotReferenceIsValid()
{
	return IsValid(slotRef);
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

