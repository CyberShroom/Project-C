// Fill out your copyright notice in the Description page of Project Settings.


#include "CargoShip.h"

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
}

void ACargoShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVector::Dist(GetActorLocation(), UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation()) <= 500)
	{
		interactWidget->SetTintColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}
	else
	{
		interactWidget->SetTintColorAndOpacity(FLinearColor(1, 1, 1, 0));
	}
}
