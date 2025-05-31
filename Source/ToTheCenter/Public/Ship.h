// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SDIO_Item.h"
#include "Camera/CameraComponent.h"
#include "Inventory.h"
#include "TTCMovementComponent.h"
#include "TTCHealthComponent.h"
#include "TTC_Enums.h"
#include "Ship.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHullDamage, float, newCurrentHull, float, damageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHullHeal, float, newCurrentHull, float, amountHealed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShieldDamage, float, newShield, float, damageTaken);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldBreak);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldRegenStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShieldRegenTick, float, newShield);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldRegenEnd);

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
	/// How much time must pass before accruing time again
	/// </summary>
	UPROPERTY()
	float outOfCombatTimer = 0;

	/// <summary>
	/// Calls all ships on all clients and server to update the hp values of the ship
	/// </summary>
	/// <param name="amount">amount of damage or healing dealt. damage is negative and healing is positive values.</param>
	/// <param name="newValue">the new value of the hp pool. Not actually applied here, purely cosmetic.</param>
	/// <param name="pool">the pool that was affected by this health change.</param>
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_NotifyHealthChange(float amount, float newValue, EHealthPools pool);

	/// <summary>
	/// Runs a timer that decrements the ooc timer. 
	/// </summary>
	UFUNCTION()
	void OutOfCombatTimer();

	/// <summary>
	/// Checks for when regen ends so it can run the event
	/// </summary>
	UFUNCTION()
	void CheckForRegenEnd();

	/// <summary>
	/// Sets the ooc timer value.
	/// </summary>
	/// <param name="newValue">new value to set to.</param>
	UFUNCTION()
	void SetOutOfCombatTimer(float newValue);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	USceneComponent* scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UTTCMovementComponent* movementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UTTCHealthComponent* healthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UStaticMeshComponent* shipMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// Contains movement stats
	/// </summary>
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships speed stats."))
	FMovementStats movementStats{};

	/// <summary>
	/// Determines the number of movement ticks per second. 0.1 = 10 t/s. 1.0 = 1 t/s
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Determines the number of movement ticks per second. 0.1 = 10 t/s. 1.0 = 1 t/s"))
	float movementDuration = 0.2;

	/// <summary>
	/// The ships health
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships hull"))
	FHealthPool hull{ 90,90 };

	/// <summary>
	/// The ships shields
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships shields"))
	FHealthPool shield{ 45,45 };

	/// <summary>
	/// The ships armor
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "The ships armor"))
	FHealthPool armor{ 15,90 };

	/// <summary>
	/// How much shield to regain per second
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "How much the shield regens per second"))
	float shieldRegenPerSecond = 15;

public:	
	///////////////////////////////////////////////////EVENTS////////////////////////////////////////////////////////////

	/// <summary>
	/// Called when the ship takes hull damage.
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

	/// <summary>
	/// Called when the ships start regening shields
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships starts to regen shield."))
	FShieldRegenStart onShieldRegenStart;

	/// <summary>
	/// Called when the ships shield regen ends
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the ships shield regen ends."))
	FShieldRegenEnd onShieldRegenEnd;

	/// <summary>
	/// Called when the shield regens during a tick. Mostly used to update ui bars.
	/// </summary>
	UPROPERTY(BlueprintCallable, Category = "Ship", meta = (Tooltip = "Called when the shield regens during a tick. Mostly used to update ui bars."))
	FShieldRegenTick onShieldRegenTick;

	/////////////////////////////////////////////EVENTS END//////////////////////////////////////////////////////////////



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

	/// <summary>
	/// Adds a vector to the movement timeline
	/// </summary>
	/// <param name="forwardVector">Vector to add.</param>
	/// <param name="isStop">Whether this command will stop movement or not</param>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Adds a vector to the movement timeline."))
	void AddVectorToTimeline(FVector forwardVector, bool isStop = false);

	/// <summary>
	/// Adds a rotation to the movement timeline.
	/// </summary>
	/// <param name="rotation">Rotation to add</param>
	/// <param name="isStop">Whether this command will stop movement or not</param>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Adds a rotation to the movement timeline."))
	void AddRotationToTimeline(FRotator rotation, bool isStop = false);

	/// <summary>
	/// Moves the ship.
	/// If done on server, the server will automatically use the whatever values are in the ships timeline. JoystickValue is only used on the client.
	/// </summary>
	/// <param name="joystickValue">Input value</param>
	/// <returns>Returns true if no errors occur.</returns>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Moves the ship."))
	bool MoveShip(float joystickValue);

	/// <summary>Contains logic for how this ship should turn.</summary>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Tells the ship to turn."))
	bool TurnShip(float joystickValue);

	/// <summary>
	/// Stops the ships movement
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Stops the ship."))
	void StopShip();

	/// <summary>
	/// Stops the ships rotation
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Stops the ships rotation."))
	void StopShipRotation();

	/// <summary>
	/// Returns the forward vector, corrected to match the ship mesh.
	/// </summary>
	/// <returns>Returns the forward vector, corrected to match the ship mesh.</returns>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Returns the ships corrected forward vector."))
	FVector GetCorrectedForwardVector();

	/// <summary>
	/// Calculates the amount to move forward by
	/// </summary>
	/// <param name="joystickValue"></param>
	/// <param name="vector">Base vector to add the amount to</param>
	/// <returns>Returns the base vector + the amount to add</returns>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Returns a forward vector for movement purposes."))
	FVector CalculateMovementVector(float joystickValue, FVector vector);

	/// <summary>
	/// Calculates the amount to add to a rotation
	/// </summary>
	/// <param name="joystickValue"></param>
	/// <returns>Returns a rotator that only contains the amount, not the base rotation!</returns>
	UFUNCTION(BlueprintCallable, Category = "Ship | Movement", meta = (Tooltip = "Calculates the amount to add to a rotation."))
	FRotator CalculateRotation(float joystickValue);

	/// <summary>
	/// Initializes some values from the ship and finishes initializing the movement component
	/// </summary>
	/// <param name="useReplication">Whether this instance of the ship should use the replicated location from the movement component. False = CLIENT OWNED INSTANCE.</param>
	/// <param name="canRun">Whether this instance of the ship can use the run function from the movement component. False = CLIENT OWNED INSTANCE.</param>
	UFUNCTION()
	void Initialize(bool useReplication, bool canRun);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called to replicate movement
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
