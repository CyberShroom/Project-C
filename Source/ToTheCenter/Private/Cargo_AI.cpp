// Fill out your copyright notice in the Description page of Project Settings.


#include "Cargo_AI.h"

void ACargo_AI::Initialize()
{
	cargoShip = Cast<AShip>(GetPawn());

	if (IsValid(cargoShip))
	{
		bIsTickable = true;
		cargoShip->Initialize(false, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACargo_AI::Initialize);
	}
}

void ACargo_AI::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void ACargo_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTickable)
	{
		cargoShip->MoveShip(1.0);
	}
}
