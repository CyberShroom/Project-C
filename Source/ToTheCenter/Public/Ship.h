// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SDIO_Item.h"
#include "Camera/CameraComponent.h"
#include "Inventory.h"
#include "Ship.generated.h"

UCLASS()
class TOTHECENTER_API AShip : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	USceneComponent* scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UCameraComponent* camera;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>The ships move speed</summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships move speed."))
	float moveSpeed = 1.0;

	/// <summary>The ships turn speed</summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships turn speed."))
	float turnSpeed = 1.0;

	/// <summary>Contains logic for how this ship should move.</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Ship", meta = (Tooltip = "Tells the ship to move. joystickValue is a modifier between 0 and 1."))
	virtual void MoveShip(float joystickValue);

	/// <summary>Contains logic for how this ship should turn.</summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Ship", meta = (Tooltip = "Tells the ship to turn. joystickValue is a modifier between 0 and 1."))
	virtual void TurnShip(float joystickValue);

public:	
	/// <summary>An array of items used by the ship. even are projectiles and odd are weapons.</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The hotbar inventory. 0-3 are projectiles and 4-7 are weapons."))
	UInventory* shipInventory;

	/// <summary>
	/// Current size of the hotbar
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "Current size of the hotbar"))
	uint8 currentHotbarSize = 2;

	// Sets default values for this pawn's properties
	AShip();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called to replicate movement
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

};
