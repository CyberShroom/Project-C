// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ship.h"
#include "Cargo_AI.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API ACargo_AI : public AAIController
{
	GENERATED_BODY()

private:
	/// <summary>
	/// Reference to the pawn's AShip subclass
	/// </summary>
	UPROPERTY()
	AShip* cargoShip;

	/// <summary>
	/// Holds the target location of the ship. This is unused but needed because the movement function has an output parameter.
	/// </summary>
	UPROPERTY()
	FVector targetLocation = FVector::ZeroVector;
	
	/// <summary>
	/// Determines if the tick function is allowed to run
	/// </summary>
	UPROPERTY()
	bool bIsTickable = false;

	UFUNCTION()
	void Initialize();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
