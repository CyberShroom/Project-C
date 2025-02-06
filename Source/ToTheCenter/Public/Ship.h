// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SDIO_Item.h"
#include "Camera/CameraComponent.h"
#include "Inventory.h"
#include "Components/InterpToMovementComponent.h"
#include "Ship.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHullDamage, float, newCurrentHull, float, damageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHullHeal, float, newCurrentHull, float, amountHealed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShieldDamage, float, newShield, float, damageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldBreak);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArmorDamage, float, newArmor, float, damageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArmorGained, float, newArmor, float, armorGained);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArmorBreak);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxHullChanged, float, newMaxHull);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxShieldChanged, float, newMaxShield);

UCLASS()
class TOTHECENTER_API AShip : public APawn
{
	GENERATED_BODY()

private:
	/// <summary>
	/// The ships current health.
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_currentHull)
	float currentHull = 90;

	/// <summary>
	/// The ships max health.
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_maxHull)
	float maxHull = 90;

	/// <summary>
	/// The ships armor.
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_armor)
	float armor = 15;

	/// <summary>
	/// The ships max shields.
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_maxShield)
	float maxShield = 45;

	/// <summary>
	/// The ships current shields.
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_currentShield)
	float currentShield = 45;

	/// <summary>
	/// How much shield to regain per second
	/// </summary>
	UPROPERTY()
	float shieldRegenPerSecond = 15;

	/// <summary>
	/// How much time has accrued for regenerating the shield
	/// </summary>
	UPROPERTY()
	float shieldRegenTimer = 0;

	/// <summary>
	/// How much time must pass before accruing time again
	/// </summary>
	UPROPERTY()
	float outOfCombatTimer = 0;

	/// <summary>
	/// The last target location set by the server.
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_targetLocation)
	FVector targetLocation;

	/// <summary>
	/// The last predicted location of movement
	/// </summary>
	UPROPERTY()
	FVector lastPredictedLocation;

	/// <summary>
	/// Offset vector to add to the next predicted location to move the client in line with the server
	/// </summary>
	UPROPERTY()
	FVector offset = FVector(0, 0, 0);

	/// <summary>
	/// How many movement checks to wait for before checking for desync again.
	/// </summary>
	UPROPERTY()
	int errorCheckWait = 0;

	/// <summary>
	/// Called when max hull replicates to clients.
	/// </summary>
	UFUNCTION()
	void OnRep_maxHull();

	/// <summary>
	/// Called when hull replicates to clients.
	/// </summary>
	UFUNCTION()
	void OnRep_currentHull();

	/// <summary>
	/// Called when max shield replicates to clients.
	/// </summary>
	UFUNCTION()
	void OnRep_maxShield();

	/// <summary>
	/// Called when shield replicates to clients.
	/// </summary>
	UFUNCTION()
	void OnRep_currentShield();

	/// <summary>
	/// Called when armor replicates to clients.
	/// </summary>
	UFUNCTION()
	void OnRep_armor();

	/// <summary>
	/// Called when target location replicates to clients
	/// </summary>
	UFUNCTION()
	void OnRep_targetLocation();

	/// <summary>
	/// Runs the related event on the client side for cosmetic purposes.
	/// </summary>
	/// <param name="isDamage">If true, run onDamage. If false, Run onHeal.</param>
	UFUNCTION(Client, Unreliable)
	void ClientRPC_NotifyClientOfHullChange(bool isDamage, float amount);

	/// <summary>
	/// Runs the related event on the client side for cosmetic purposes.
	/// </summary>
	UFUNCTION(Client, Unreliable)
	void ClientRPC_NotifyClientOfShieldChange(float amount);

	/// <summary>
	/// Runs the related event on the client side for cosmetic purposes.
	/// </summary>
	UFUNCTION(Client, Unreliable)
	void ClientRPC_NotifyClientOfArmorChange(bool isDamage, float amount);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	USceneComponent* scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UInterpToMovementComponent* movementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UStaticMeshComponent* shipMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>The ships move speed</summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships move speed."))
	float moveSpeed = 1.0;

	/// <summary>The ships turn speed</summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships turn speed."))
	float turnSpeed = 1.0;

	/// <summary>
	/// Determines if the client is allowed to send movement commands to the server.
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "Determines if the client is allowed to send movement commands to the server."))
	bool bAwaitingServerMovementTick = false;

	/// <summary>Contains logic for how this ship should turn.</summary>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Tells the ship to turn. joystickValue is a modifier between 0 and 1."))
	virtual void TurnShip(float joystickValue);

	UFUNCTION(BlueprintPure, Category = "Ship")
	FVector GetLastPredictedLocation();

public:	
	/// <summary>
	/// Called when the ship takes hull takes damage.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ship takes hull damage."))
	FHullDamage onHullDamage;

	/// <summary>
	/// Called when the ship takes shield damage
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ship shield takes damage."))
	FShieldDamage onShieldDamage;

	/// <summary>
	/// Called when the ship takes armor damage
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ship armor takes damage."))
	FArmorDamage onArmorDamage;

	/// <summary>
	/// Called when the ship takes hull heals.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ship hull heals."))
	FHullHeal onHullHeal;

	/// <summary>
	/// Called when the ships shield becomes 0
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships shield becomes 0"))
	FShieldBreak onShieldBreak;

	/// <summary>
	/// Called when the ships armor becomes 0
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships armor becomes 0"))
	FArmorBreak onArmorBreak;

	/// <summary>
	/// Called when the ships max hull changes.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships max hull changes."))
	FMaxHullChanged onMaxHullChanged;

	/// <summary>
	/// Called when the ships max shield changes.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships max shield changes."))
	FMaxShieldChanged onMaxShieldChanged;

	/// <summary>
	/// Called when the ships gains armor.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships gains armor."))
	FArmorGained onGainArmor;

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

	/// <summary>
	/// Damages the ship. Starts with shield, then armor, then hull.
	/// </summary>
	/// <param name="damage">Damage to deal</param>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Damages the ship. Starts with shield, then armor, then hull."))
	void ShipTakeDamage(float damage);

	/// <summary>
	/// Heals the ships hull.
	/// </summary>
	/// <param name="heal">amount to heal</param>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Heals the ships hull."))
	void HealHull(float heal);

	UFUNCTION(BlueprintPure, Category = "Ship")
	float GetMaxHull();

	UFUNCTION(BlueprintPure, Category = "Ship")
	float GetMaxShield();

	UFUNCTION(BlueprintPure, Category = "Ship")
	float GetArmor();

	UFUNCTION(BlueprintPure, Category = "Ship")
	FVector GetMovementTargetLocation();

	/// <summary>Moves the ship forward based on its actor location.</summary>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Moves the ship forward based on its actor location. joystickValue is a modifier between 0 and 1."))
	bool MoveShip_Actor(float joystickValue);

	/// <summary>
	/// Moves the ship forward based on its target location.
	/// </summary>
	/// <param name="joystickValue">modifier between 0 and 1</param>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Moves the ship forward based on its target location."))
	bool MoveShip_Target(float joystickValue);

	/// <summary>
	/// Checks for error between the 2 given points and sets an offset to fix it.
	/// </summary>
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Ship", meta = (Tooltip = "Checks for error between the 2 given points and sets an offset to fix it."))
	void ClientRPC_CheckForError(FVector trueLocation, FVector predictedLocation);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called to replicate movement
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

};
