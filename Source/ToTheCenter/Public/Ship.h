// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SDIO_Item.h"
#include "Camera/CameraComponent.h"
#include "Inventory.h"
#include "TTCMovementComponent.h"
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
	// HEALTH SYSTEM //

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


	// MOVEMENT SYSTEM //

	/// <summary>
	/// The last target rotation set by the server.
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_targetRotation)
	FRotator targetRotation;

	/// <summary>
	/// The last target rotation set by the server.
	/// </summary>
	UPROPERTY()
	FRotator lastPredictedRotation;

	/// <summary>
	/// The interpolation speed needed to rotate at a 0.2 second duration
	/// </summary>
	UPROPERTY()
	float rotationSpeed;

	/// <summary>
	/// Offset vector to add to the next predicted location to move the client in line with the server
	/// </summary>
	UPROPERTY()
	FVector locationOffset = FVector(0, 0, 0);

	/// <summary>
	/// Offset rotator to add to the next predicted rotation to move the client in line with the server
	/// </summary>
	UPROPERTY()
	FRotator rotationOffset = FRotator(0, 0, 0);

	/// <summary>
	/// How many movement checks to wait for before checking for desync again.
	/// </summary>
	UPROPERTY()
	int locationErrorCheckWait = 0;

	/// <summary>
	/// How many rotation checks to wait for before checking for desync again.
	/// </summary>
	UPROPERTY()
	int rotationErrorCheckWait = 0;

	/// <summary>
	/// Flag that immediatly stops the objects rotation
	/// </summary>
	UPROPERTY()
	bool bStopRotation = false;

	/// <summary>
	/// Called when target rotation replicates to clients
	/// </summary>
	UFUNCTION()
	void OnRep_targetRotation();

	/// <summary>
	/// Rotates the entity
	/// </summary>
	UFUNCTION()
	void Rotate(FRotator newRotation, float elapsedTime);


	// OTHER //

	/// <summary>
	/// How much time must pass before accruing time again
	/// </summary>
	UPROPERTY()
	float outOfCombatTimer = 0;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	USceneComponent* scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UTTCMovementComponent* movementComponent;

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
	/// Determines the number of movement ticks per second. 0.1 = 10 t/s. 1.0 = 1 t/s
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Determines the number of movement ticks per second. 0.1 = 10 t/s. 1.0 = 1 t/s"))
	float movementDuration = 0.2;

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
	FRotator GetTargetRotation();

	UFUNCTION(BlueprintPure, Category = "Ship")
	float GetMovementDuration();

	/// <summary>
	/// Moves the ship.
	/// If done on server, the server will automatically use the whatever values are in the ships timeline. JoystickValue is only used on the client.
	/// </summary>
	/// <param name="joystickValue">Input value</param>
	/// <returns>Returns true if no errors occur.</returns>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Moves the ship."))
	bool MoveShip(float joystickValue);

	UFUNCTION()
	void AddVectorToTimeline(FVector forwardVector);

	/// <summary>Contains logic for how this ship should turn.</summary>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Tells the ship to turn. joystickValue is a modifier between 0 and 1."))
	bool TurnShip(float joystickValue, bool useTarget, FRotator& predictedRotation);

	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Stops the ship."))
	void StopShip(FVector& predictedLocation);

	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Stops the ships rotation."))
	void StopShipRotation(FRotator& predictedRotation);

	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Returns the ships corrected forward vector."))
	FVector GetCorrectedForwardVector();

	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Returns a forward vector for movement purposes."))
	FVector CalculateMovementForwardVector(float joystickValue);

	UFUNCTION()
	FVector GetTargetPosition();

	/// <summary>
	/// Temporary solution to a problem. Should be changed later.
	/// </summary>
	/// <param name="value"></param>
	UFUNCTION()
	void SetMovementReplication(bool value);

	/// <summary>
	/// Checks for error between the 2 given points and sets an offset to fix it.
	/// </summary>
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Ship", meta = (Tooltip = "Checks for error between the 2 given points and sets an offset to fix it."))
	void ClientRPC_CheckForLocationError(FVector trueLocation);

	/// <summary>
	/// Checks for error between the 2 given rotations and sets an offset to fix it.
	/// </summary>
	UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Ship", meta = (Tooltip = "Checks for error between the 2 given rotations and sets an offset to fix it."))
	void ClientRPC_CheckForRotationError(FRotator trueRotation, FRotator predictedRotation);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called to replicate movement
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
