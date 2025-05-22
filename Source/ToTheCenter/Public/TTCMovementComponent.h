// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InterpToMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "TTCMovementComponent.generated.h"

USTRUCT()
struct FMovementState
{
	GENERATED_BODY()

	//The vector contained in the state
	FVector vector;

	//If true, this vector is a stop command
	bool isStop;

	//If isStop is true, this flag determines if it is a valid stop vector.
	bool isVerified = false;
};

/**
 * 
 */
UCLASS()
class TOTHECENTER_API UTTCMovementComponent : public UInterpToMovementComponent
{
	GENERATED_BODY()
	
private:
	/// <summary>
	/// The position the object is attempting to move to
	/// </summary>
	UPROPERTY()
	FVector targetPosition;

	/// <summary>
	/// Copy of target position that is used for replication purposes
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_targetPosition)
	FVector replicatedPosition;

	UPROPERTY()
	FVector offset = FVector::Zero();

	/// <summary>
	/// A timeline of previous movement commands.
	/// On a client, this contains a list of predicted locations that are compared to the locations coming from the server. These vectors determine desync.
	/// On a server, this contians a list of forward vectors that are used to tell the ship where to move at step by step. These vectors allow movement to look smooth even under bad network conditions.
	/// </summary>
	UPROPERTY()
	TArray<FMovementState> movementTimeline;

	/// <summary>
	/// When an offset is applied, desync detection is disabled. This index value determines when desync detection should be re-enabled to prevent over correction.
	/// </summary>
	UPROPERTY()
	int offsetIndex = 0;

	UFUNCTION()
	void OnRep_targetPosition();

	UFUNCTION()
	void SetTargetPosition(FVector newPosition);

public:
	/// <summary>
	/// Should this component use the replicated position value? This must be false for locally owned pawns.
	/// </summary>
	UPROPERTY()
	bool useReplicatedPosition = true;

	UPROPERTY()
	bool debugMovement = true;

	UFUNCTION()
	void AddToTimeline(FVector newVector, bool isStop = false);

	UFUNCTION()
	void RemoveNextFromTimeline();

	UFUNCTION()
	FMovementState GetNextFromTimeline();

	UFUNCTION()
	FVector GetTargetPosition();

	UFUNCTION()
	void CheckForDesync(FVector finalLocation);

	/// <summary>
	/// Move the object forward and backward. Returns true on success.
	/// </summary>
	UFUNCTION()
	bool Move(FVector forwardVector, FVector currentPosition, bool isServer);

	/// <summary>
	/// Turn the object right and left
	/// </summary>
	UFUNCTION()
	void Turn();

	/// <summary>
	/// Stop the objects movement
	/// </summary>
	UFUNCTION()
	bool Stop(FVector currentPosition, bool isServer);

	/// <summary>
	/// Draws debug lines for movement.
	/// </summary>
	UFUNCTION()
	void DrawDebugLines(FVector currentPosition);

	UFUNCTION()
	void Initialize(FVector originalPosition);

	// Called to replicate movement
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
