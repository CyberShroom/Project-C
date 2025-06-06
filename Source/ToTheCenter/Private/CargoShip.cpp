// Fill out your copyright notice in the Description page of Project Settings.


#include "CargoShip.h"

void ACargoShip::GenerateFrameItem(AItem_Frame* frame)
{
	if (!IsValid(frame)) return;

	if (frame->SlotReferenceIsValid())
	{
		frame->Multicast_SetItem(containedItem);
		Destroy();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, frame]() {GenerateFrameItem(frame); });
	}
}

ACargoShip::ACargoShip()
{
	cargoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cargoMesh"));
	cargoMesh->SetupAttachment(scene);

	interactWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("interactWidget"));
	interactWidget->SetupAttachment(scene);

	movementStats.baseSpeed = 25;
}

void ACargoShip::BeginPlay()
{
	Super::BeginPlay();

	containedItem = GetGameInstance()->GetSubsystem<UItemRegistry>()->GetRandomItemID();
}

void ACargoShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IInteractable_Interface::CheckProx(GetActorLocation(), interactWidget, GetWorld());
}

void ACargoShip::Interact_Implementation(APlayerController* controller)
{
	//Set spawn params
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//Spawn an item frame in world
	AActor* newFrame = GetWorld()->SpawnActor<AActor>(
		frameRef,
		GetActorLocation(),
		FRotator::ZeroRotator,
		params
	);

	//Cast to item frame
	AItem_Frame* frame = Cast<AItem_Frame>(newFrame);

	//Set the item in the new item frame
	GenerateFrameItem(frame);
}
