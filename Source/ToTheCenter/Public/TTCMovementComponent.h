// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InterpToMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "TTCMovementComponent.generated.h"

UENUM() //Determines the type of movement and how it will be handled
enum FMovementType
{
	INVALID, //This movement state is invalid and should be ignored
	MOVE, //Uses vector to store forward vector on server and location vector on client
	ROTATE //Uses rotator to store rotation vector on server and mesh rotation on client
};


USTRUCT(BlueprintType) //Contains data on the movement.
struct FMovementState
{
	GENERATED_USTRUCT_BODY()

	//The vector contained in the state
	FVector vector;

	//The rotation contained in the state
	FRotator rotation;

	//The type of movement being performed
	FMovementType type;

	//If true, this vector is a stop command
	bool isStop;

	//If isStop is true, this flag determines if it is a valid stop vector.
	bool isVerified = false;

	//When used with rotation, determines if the object is rotating clockwise or countclockwise.
	bool isClockwise;

	//Used by the run function to determine if a command is stalling.
	float elapsedTime = 0;

	//Empty constructor. Should be ignored.
	FMovementState()
	{
		vector = FVector::Zero();
		rotation = FRotator::ZeroRotator;
		type = FMovementType::INVALID;
	}

	//Construct a movement state
	FMovementState(FVector newVector, bool stop, bool verification = false)
	{
		vector = newVector;
		isStop = stop;
		type = FMovementType::MOVE;
		isVerified = verification;
	}

	//Construct a rotation state
	FMovementState(FRotator newRotation, bool stop, bool verification = false, bool clockwise = false)
	{
		rotation = newRotation;
		isStop = stop;
		type = FMovementType::ROTATE;

		//Only set clockwise boolean if the rotation is verified
		if (verification)
		{
			isClockwise = clockwise;
		}
	}
};

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
	/// The rotation the object is attempting to rotate to
	/// </summary>
	UPROPERTY()
	FRotator targetRotation;

	/// <summary>
	/// The rotation prior to using the rotate command. Used to determine if a stop rotation command is valid.
	/// </summary>
	UPROPERTY()
	FRotator priorRotation;

	/// <summary>
	/// Copy of target position that is used for replication purposes
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_targetPosition)
	FVector replicatedPosition;

	/// <summary>
	/// Copy of target position that is used for replication purposes
	/// </summary>
	UPROPERTY(ReplicatedUsing = OnRep_targetRotation)
	FRotator replicatedRotation;

	/// <summary>
	/// Offset to apply to the next movement input to fix desync over time
	/// </summary>
	UPROPERTY()
	FVector movementOffset = FVector::Zero();

	/// <summary>
	/// Offset to apply to the next rotation input to fix desync over time
	/// </summary>
	UPROPERTY()
	FRotator rotationOffset = FRotator::ZeroRotator;

	/// <summary>
	/// A timeline of previous movement commands.
	/// On a client, this contains a list of predicted locations and predicted Rotations that are compared to the locations and rotations coming from the server. These vectors determine desync.
	/// On a server, this contians a list of forward vectors and rotations that are used to tell the ship where to move at step by step. These vectors allow movement to look smooth even under bad network conditions.
	/// </summary>
	UPROPERTY()
	TArray<FMovementState> movementTimeline;

	/// <summary>
	/// When an offset is applied, desync detection is disabled. This index value determines when desync detection should be re-enabled to prevent over correction.
	/// </summary>
	UPROPERTY()
	int movementOffsetIndex = 0;

	/// <summary>
	/// When an offset is applied, desync detection is disabled. This index value determines when desync detection should be re-enabled to prevent over correction.
	/// </summary>
	UPROPERTY()
	int rotationOffsetIndex = 0;

	/// <summary>
	/// Whether the Run function is waiting on an entry to be added to the timeline. Prevents the run function from using system resources when it has nothing to run.
	/// </summary>
	UPROPERTY()
	bool awaitTimeline = true;

	/// <summary>
	/// Should this component use the replicated position and rotation value? This must be false for locally owned pawns.
	/// </summary>
	UPROPERTY()
	bool useReplicatedPosition = true;

	/// <summary>
	/// Used to stop ongoing rotation
	/// </summary>
	UPROPERTY()
	bool interruptRotation = false;

	/// <summary>
	/// Reference to the mesh component of the parent actor
	/// </summary>
	UPROPERTY()
	UStaticMeshComponent* meshRef;

	UFUNCTION()
	void OnRep_targetPosition();

	UFUNCTION()
	void OnRep_targetRotation();

	/// <summary>
	/// THE RUN FUNCTION
	/// Runs the first command in the timeline.
	/// </summary>
	UFUNCTION()
	void RunNextInTimeline();

	/// <summary>
	/// Rotates the actors mesh
	/// </summary>
	/// <param name="rotation">The target rotation to rotate towards</param>
	/// <param name="elapsedTime">The amount of time in seconds that has passed since rotation started</param>
	/// <param name="rotationSpeed">How fast the rotation will be performed</param>
	UFUNCTION()
	void Rotate(FRotator rotation, float elapsedTime, float rotationSpeed);

	/// <summary>
	/// Checks for error between the 2 given points and sets an offset to fix it.
	/// </summary>
	UFUNCTION(Client, Reliable)
	void ClientRPC_CheckForLocationError(FVector trueLocation);

	/// <summary>
	/// Checks for error between the 2 given rotations and sets an offset to fix it.
	/// </summary>
	UFUNCTION(Client, Reliable)
	void ClientRPC_CheckForRotationError(FRotator trueRotation);

public:
	/// <summary>
	/// constructor to set replication
	/// </summary>
	UTTCMovementComponent();

	/// <summary>
	/// Toggles debug lines for movement.
	/// </summary>
	UPROPERTY()
	bool debugMovement = false;

	/// <summary>
	/// Add a forward vector(SERVER) or predicted location(CLIENT) to the timeline.
	/// </summary>
	/// <param name="newVector">Vector to add</param>
	/// <param name="isStop">Is this a move command or a stop command? (SERVER ONLY)</param>
	UFUNCTION(BlueprintCallable, Category = "Movement Component", meta = (Tooltip = "Adds a forward vector (SERVER) or predicted location (CLIENT) to the timeline."))
	void AddVectorToTimeline(FVector newVector, bool isStop = false);

	/// <summary>
	/// Add a rotation vector (SERVER) or a predicted rotation(CLIENT) to the timeline.
	/// </summary>
	/// <param name="newRotation">Rotation to add</param>
	/// <param name="isStop">Is this a rotate command or a stop command? (SERVER ONLY)</param>
	UFUNCTION(BlueprintCallable, Category = "Movement Component", meta = (Tooltip = "Adds a rotation (SERVER) or predicted rotation (CLIENT) to the timeline."))
	void AddRotationToTimeline(FRotator newRotation, bool isStop = false);

	/// <summary>
	/// Remove the FMovementState at [0]
	/// </summary>
	UFUNCTION()
	void RemoveNextFromTimeline();

	/// <summary>
	/// Retreive the FMovementState at [0]
	/// </summary>
	/// <returns>FMovementState at [0]</returns>
	UFUNCTION(BlueprintPure, Category = "Movement Component", meta = (Tooltip = "Retreive the FMovementState at [0]"))
	FMovementState GetNextFromTimeline();

	/// <summary>
	/// Return the actors target position (Position it is trying to go to.)
	/// </summary>
	/// <returns>target position</returns>
	UFUNCTION(BlueprintPure, Category = "Movement Component", meta = (Tooltip = "Return the actors target position (Position it is trying to go to.)"))
	FVector GetTargetPosition();

	/// <summary>
	/// Return the actors target rotation (Rotation it is trying to rotate to.)
	/// </summary>
	/// <returns>target rotation</returns>
	UFUNCTION(BlueprintPure, Category = "Movement Component", meta = (Tooltip = "Return the actors target position (Position it is trying to go to.)"))
	FRotator GetTargetRotation();

	/// <summary>
	/// Sets the target and replicated target position
	/// </summary>
	/// <param name="newPosition">Postion to set it to</param>
	UFUNCTION(BlueprintCallable, Category = "Movement Component", meta = (Tooltip = "Sets the target and replicated target position"))
	void SetTargetPosition(FVector newPosition);

	/// <summary>
	/// Sets the target and replicated target rotation
	/// </summary>
	/// <param name="rotation">Rotation to set it to</param>
	UFUNCTION(BlueprintCallable, Category = "Movement Component", meta = (Tooltip = "Sets the target and replicated target rotation"))
	void SetTargetRotation(FRotator rotation);

	/// <summary>
	/// Moves the actor
	/// </summary>
	/// <param name="forwardVector">Direction and amount to move</param>
	/// <param name="currentPosition">Actor's location at the start of movement</param>
	/// <param name="isServer">True if ran on server</param>
	/// <returns>True on success. False on failure or error.</returns>
	UFUNCTION(BlueprintCallable, Category = "Movement Component", meta = (Tooltip = "Moves the actor."))
	bool Move(FVector forwardVector, FVector currentPosition, bool isServer);

	/// <summary>
	/// Calculates and starts turning the actor.
	/// </summary>
	/// <param name="rotation">Amount to add to the rotation</param>
	/// <param name="isServer">True if ran on server.</param>
	/// <returns>Returns True on success. False on failure or error.</returns>
	UFUNCTION(BlueprintCallable, Category = "Movement Component", meta = (Tooltip = "Calculates and starts turning the actor."))
	bool Turn(FRotator rotation, bool isServer);

	/// <summary>
	/// Stops the actors movement
	/// </summary>
	/// <param name="currentPosition">Location to stop at.</param>
	/// <param name="isServer">True if ran on server.</param>
	/// <returns>True on success. False on failure or error.</returns>
	UFUNCTION(BlueprintCallable, Category = "Movement Component", meta = (Tooltip = "Stops the actors movement."))
	bool StopMovement(FVector currentPosition, bool isServer);

	/// <summary>
	/// Stops the actor's rotation.
	/// </summary>
	/// <param name="isServer">True if ran on server.</param>
	/// <returns>True on success. False on failure or error.</returns>
	UFUNCTION(BlueprintCallable, Category = "Movement Component", meta = (Tooltip = "Stops the actors rotation."))
	bool StopRotation(bool isServer);

	/// <summary>
	/// Draws debug lines for movement.
	/// </summary>
	UFUNCTION()
	void DrawDebugLines(FVector currentPosition);

	UFUNCTION()
	void InitializeFromController(bool useReplicatedPostion, bool canRun);

	UFUNCTION()
	void InitializeFromShip(FVector currentPosition, UStaticMeshComponent* meshReference);

	// Called to replicate movement
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
