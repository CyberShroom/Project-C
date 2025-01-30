// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SDIO_Item.h"
#include "Camera/CameraComponent.h"
#include "Inventory.h"
#include "Ship.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHullDamage, float, newCurrentHull, float, damageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHullHeal, float, newCurrentHull, float, amountHealed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShieldDamage, float, newShield, float, damageTaken);
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
	/// Runs the related event on the client side for cosmetic purposes.
	/// </summary>
	/// <param name="isDamage">If true, run onDamage. If false, Run onHeal.</param>
	UFUNCTION(Client, Unreliable)
	void ClientRPC_NotifyClientOfHullChange(bool isDamage, float amount);

	/// <summary>
	/// Runs the related event on the client side for cosmetic purposes.
	/// </summary>
	/// <param name="isDamage">If true, run onDamage.</param>
	UFUNCTION(Client, Unreliable)
	void ClientRPC_NotifyClientOfShieldChange(float amount);

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

	/// <summary>
	/// The ships armor.
	/// </summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships armor."))
	float armor = 0;

	/// <summary>Contains logic for how this ship should move.</summary>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Tells the ship to move. joystickValue is a modifier between 0 and 1."))
	virtual void MoveShip(float joystickValue);

	/// <summary>Contains logic for how this ship should turn.</summary>
	UFUNCTION(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Tells the ship to turn. joystickValue is a modifier between 0 and 1."))
	virtual void TurnShip(float joystickValue);

public:	
	/// <summary>
	/// Called when the ship takes hull takes damage.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ship takes hull damage."))
	FHullDamage onHullDamage;

	/// <summary>
	/// Called when the ship takes hull heals.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ship hull heals."))
	FHullHeal onHullHeal;

	/// <summary>
	/// Called when the ships max hull changes.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships max hull changes."))
	FMaxHullChanged onMaxHullChanged;

	/// <summary>
	/// Called when the ship takes shield damage
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ship shield takes damage."))
	FShieldDamage onShieldDamage;

	/// <summary>
	/// Called when the ships max shield changes.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships max shield changes."))
	FMaxShieldChanged onMaxShieldChanged;

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

	UFUNCTION(BlueprintCallable, Category = "Ship")
	float GetMaxHull();

	UFUNCTION(BlueprintCallable, Category = "Ship")
	float GetMaxShield();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called to replicate movement
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

};
