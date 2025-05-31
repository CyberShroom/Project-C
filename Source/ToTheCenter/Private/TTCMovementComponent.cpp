// Fill out your copyright notice in the Description page of Project Settings.


#include "TTCMovementComponent.h"

bool UTTCMovementComponent::Move(FVector forwardVector, FVector currentPosition, bool isServer)
{
	//Do not move if the targetLocation and object location do not match
	if (targetPosition != currentPosition)
	{
		return false;
	}

	//Do not move if the forward vector is not set
	if (forwardVector.ContainsNaN())
	{
		forwardVector = FVector::Zero();
	}

	//Update the target position
	SetTargetPosition(targetPosition + forwardVector + movementOffset);
	movementOffset = FVector::Zero();

	//Set the interpolation values
	StopMovementImmediately();
	ResetControlPoints();
	AddControlPointPosition(GetOwner()->GetActorLocation(), false);
	AddControlPointPosition(targetPosition, false);
	FinaliseControlPoints();
	RestartMovement();

	//if client and locally owned, save the target position to check for desync later
	if (!isServer && GetOwner()->HasNetOwner())
	{
		AddVectorToTimeline(targetPosition);
	}

	return true;
}

void UTTCMovementComponent::OnRep_targetPosition()
{
	//Only run if the actor uses the replicated position
	if (!useReplicatedPosition)
	{
		return;
	}

	targetPosition = replicatedPosition;

	//Set the interpolation values
	StopMovementImmediately();
	ResetControlPoints();
	AddControlPointPosition(GetOwner()->GetActorLocation(), false);
	AddControlPointPosition(targetPosition, false);
	FinaliseControlPoints();
	RestartMovement();
}

void UTTCMovementComponent::OnRep_targetRotation()
{
	//Only run if the actor uses replicated rotation
	if (!useReplicatedPosition)
	{
		return;
	}

	targetRotation = replicatedRotation;

	if (IsValid(meshRef))
	{
		Rotate(targetRotation, 0, FMath::Abs(meshRef->GetRelativeRotation().Yaw - targetRotation.Yaw) / Duration);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh Reference isn't set yet. Ship will not rotate."));
	}
}

void UTTCMovementComponent::SetTargetPosition(FVector newPosition)
{
	targetPosition = newPosition;
	replicatedPosition = targetPosition;
}

void UTTCMovementComponent::RunNextInTimeline()
{
	//Run the timeline
	switch (GetNextFromTimeline().type)
	{
		//If we receive an invalid type, the timeline is empty. Await further instructions.
		case INVALID:
			awaitTimeline = true;
			return;
		//If we recieve a move type, move the ship or stop it depending on the value
		case MOVE:
			if (GetNextFromTimeline().isStop)
			{
				//Stop command. True if the actor has stopped at the designated location.
				if (StopMovement(GetOwner()->GetActorLocation(), true))
				{
					GetOwner()->SetActorLocation(targetPosition);
					RemoveNextFromTimeline();
				}
			}
			else
			{
				//Move command
				if (Move(GetNextFromTimeline().vector, GetOwner()->GetActorLocation(), true))
				{
					RemoveNextFromTimeline();
					ClientRPC_CheckForLocationError(targetPosition);
				}
				else
				{
					//Increment the timer
					FMovementState state = movementTimeline[0];
					state.elapsedTime += GetWorld()->GetDeltaSeconds();

					//If greater than the movement duration plus a small tolerance, teleport the actor to the target position.
					if (state.elapsedTime >= Duration + 0.1)
					{
						UE_LOG(LogTemp, Warning, TEXT("Movement Command Stalled! Teleporting to target position to prevent stalling!"));
						GetOwner()->SetActorLocation(targetPosition);
					}
					else
					{
						//Set the state to keep the time
						movementTimeline[0] = state;
					}
				}
			}
			break;
		case ROTATE:
			if (GetNextFromTimeline().isStop)
			{
				//Stop command. True if the actor has stopped at the designated rotation.
				if (StopRotation(true))
				{
					meshRef->SetRelativeRotation(targetRotation);
					RemoveNextFromTimeline();
				}
			}	
			else
			{
				//Rotate command
				if (Turn(GetNextFromTimeline().rotation, true))
				{
					RemoveNextFromTimeline();
					ClientRPC_CheckForRotationError(targetRotation);
				}
				else
				{
					//Increment the timer
					FMovementState state = movementTimeline[0];
					state.elapsedTime += GetWorld()->GetDeltaSeconds();

					//If greater than the movement duration plus a small tolerance, teleport the actor to the target position.
					if (state.elapsedTime >= Duration + 0.1)
					{
						UE_LOG(LogTemp, Warning, TEXT("Turn Command Stalled! Teleporting to target rotation to prevent stalling!"));
						meshRef->SetRelativeRotation(targetRotation);
					}
					else
					{
						//Set the state to keep the time
						movementTimeline[0] = state;
					}
				}
			}
			break;
	}

	//Run this every tick while the timeline has an entry
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTTCMovementComponent::RunNextInTimeline);
}

void UTTCMovementComponent::Rotate(FRotator rotation, float elapsedTime, float rotationSpeed)
{
	//If this is a new rotation, reset the interrupt.
	if (elapsedTime == 0)
	{
		interruptRotation = false;
	}
	else if(interruptRotation)
	{
		//If interruptRotation is true, stop rotating immediately.
		return;
	}

	//Keep track of time
	elapsedTime += GetWorld()->GetDeltaSeconds();

	//Increment the rotation
	meshRef->SetRelativeRotation(FMath::RInterpConstantTo(meshRef->GetRelativeRotation(), rotation, GetWorld()->GetDeltaSeconds(), rotationSpeed));

	//Continue on next tick or set the rotation when done
	if (elapsedTime >= Duration)
	{
		meshRef->SetRelativeRotation(rotation);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, rotation, elapsedTime, rotationSpeed]() {Rotate(rotation, elapsedTime, rotationSpeed); });
	}
}

void UTTCMovementComponent::ClientRPC_CheckForRotationError_Implementation(FRotator trueRotation)
{
	//If ship Yaw is not at the target yaw, don't rotate until it matches.
	if (!FMath::IsNearlyEqual(movementTimeline[0].rotation.GetNormalized().Yaw, trueRotation.GetNormalized().Yaw) && rotationOffsetIndex < 1)
	{
		//Check for edge case. If either edge case is true, do not return false. If neither edge case is true, return false.
		if (!(movementTimeline[0].rotation.GetNormalized().Yaw == 180.0f && trueRotation.GetNormalized().Yaw == -180.0f) && !(movementTimeline[0].rotation.GetNormalized().Yaw == -180.0f && trueRotation.GetNormalized().Yaw == 180.0f))
		{
			rotationOffset = trueRotation.GetNormalized() - movementTimeline[0].rotation.GetNormalized();

			UE_LOG(LogTemp, Error, TEXT("Desync Detected! Applying Offset Yaw: %f to compensate."), rotationOffset.Yaw);

			//Set the offset index equal to the index of the next position that will be added to the timeline.
			rotationOffsetIndex = movementTimeline.Num();
		}
	}

	RemoveNextFromTimeline();
}

void UTTCMovementComponent::SetTargetRotation(FRotator rotation)
{
	targetRotation = rotation;
	replicatedRotation = targetRotation;
}

void UTTCMovementComponent::ClientRPC_CheckForLocationError_Implementation(FVector trueLocation)
{
	if (!movementTimeline[0].vector.Equals(trueLocation) && movementOffsetIndex < 1)
	{
		movementOffset = trueLocation - movementTimeline[0].vector;
		UE_LOG(LogTemp, Error, TEXT("Desync Detected! Applying Offset: %f %f %f to compensate."), movementOffset.X, movementOffset.Y, movementOffset.Z);

		//Set the offset index equal to the index of the next position that will be added to the timeline.
		movementOffsetIndex = movementTimeline.Num();
	}

	RemoveNextFromTimeline();
}

UTTCMovementComponent::UTTCMovementComponent()
{
	SetIsReplicatedByDefault(true);
}

void UTTCMovementComponent::AddVectorToTimeline(FVector newVector, bool isStop)
{
	//Add the command to the timeline
	movementTimeline.Add(FMovementState{newVector, isStop});

	//If run is waiting on an entry, tell it to run.
	if (awaitTimeline)
	{
		awaitTimeline = false;
		RunNextInTimeline();
	}
}

void UTTCMovementComponent::AddRotationToTimeline(FRotator newRotation, bool isStop)
{
	//Add the command to the timeline
	movementTimeline.Add(FMovementState{ newRotation, isStop });

	//If run is waiting on an entry, tell it to run.
	if (awaitTimeline)
	{
		awaitTimeline = false;
		RunNextInTimeline();
	}
}

void UTTCMovementComponent::RemoveNextFromTimeline()
{
	movementTimeline.RemoveAt(0);
	movementOffsetIndex--;
	rotationOffsetIndex--;
}

FMovementState UTTCMovementComponent::GetNextFromTimeline()
{
	//Check if timeline is empty and return Invalid if it is
	if (movementTimeline.IsValidIndex(0))
	{
		return movementTimeline[0];
	}
	else
	{
		//Return an invalid state
		return FMovementState{};
	}
}

FVector UTTCMovementComponent::GetTargetPosition()
{
	return targetPosition;
}

FRotator UTTCMovementComponent::GetTargetRotation()
{
	return targetRotation;
}

bool UTTCMovementComponent::Turn(FRotator rotation, bool isServer)
{
	//If ship Yaw is not at the target yaw, don't rotate until it matches.
	if (FMath::IsNearlyEqual(meshRef->GetRelativeRotation().GetNormalized().Yaw, targetRotation.GetNormalized().Yaw) == false)
	{
		//Check for edge case. If either edge case is true, do not return false. If neither edge case is true, return false.
		if (!(meshRef->GetRelativeRotation().GetNormalized().Yaw == 180.0f && targetRotation.GetNormalized().Yaw == -180.0f) && !(meshRef->GetRelativeRotation().GetNormalized().Yaw == -180.0f && targetRotation.GetNormalized().Yaw == 180.0f))
		{
			return false;
		}
	}

	//Set the target rotation and reset the offset
	priorRotation = targetRotation;
	SetTargetRotation(targetRotation.GetNormalized() + rotation + rotationOffset);
	rotationOffset = FRotator::ZeroRotator;

	//Begin Rotation
	Rotate(targetRotation, 0, FMath::Abs(meshRef->GetRelativeRotation().Yaw - targetRotation.Yaw) / Duration);

	//if client and locally owned, save the target rotation to check for desync later
	if (!isServer && GetOwner()->HasNetOwner())
	{
		AddRotationToTimeline(targetRotation);
	}

	return true;
}

bool UTTCMovementComponent::StopMovement(FVector currentPosition, bool isServer)
{
	bool isAllowed = false;

	//On server, continue movement to the stop location.
	if (isServer)
	{
		//Check that we are NOT THE HOST OR AI!
		APawn* owner = Cast<APawn>(GetOwner());

		if (!owner)
		{
			//Failed to get pawn, throw out the command
			UE_LOG(LogTemp, Error, TEXT("Stop Command failed to cast to APawn! The vector will be thrown out!"));
			RemoveNextFromTimeline();
			return false;
		}

		//Check that we are NOT THE HOST OR AI!
		if (owner->GetController()->IsLocalPlayerController() || !owner->IsPlayerControlled())
		{
			//This is the host or AI
			isAllowed = true;
		}
		else
		{
			//This is not the host nor AI
			//If it isn't verified we need to verify it
			if (!GetNextFromTimeline().isVerified)
			{
				//Validate that argument is valid
				FVector direction = (ControlPoints[1].PositionControlPoint - ControlPoints[0].PositionControlPoint).GetSafeNormal();
				FVector toPoint = GetNextFromTimeline().vector - ControlPoints[0].PositionControlPoint;

				//Check if point lies on the line
				float dot = FVector::DotProduct(toPoint.GetSafeNormal(), direction);
				bool isAligned = FMath::IsNearlyEqual(dot, 1.0f, 1.0f);

				//Check if projected length is between 0 and line length
				float projectedLength = FVector::DotProduct(toPoint, direction);
				float length = FVector::Dist(ControlPoints[0].PositionControlPoint, ControlPoints[1].PositionControlPoint);

				bool isBetween = projectedLength >= -1.0f && projectedLength <= length + 1.0f;

				if (isAligned && isBetween)
				{
					//Recreate the state as verified
					movementTimeline[0] = FMovementState{ movementTimeline[0].vector, true, true };
				}
				else
				{
					//Verification failed. Throw out the value.
					UE_LOG(LogTemp, Error, TEXT("Stop Command had an invalid vector! The vector will be thrown out!"));
					RemoveNextFromTimeline();
					return false;
				}
			}

			//Now we stop the ship if it passes the stop point
			FVector direction = (ControlPoints[1].PositionControlPoint - ControlPoints[0].PositionControlPoint).GetSafeNormal();
			float stopDistance = FVector::DotProduct(GetNextFromTimeline().vector - ControlPoints[0].PositionControlPoint, direction);
			float currentDistance = FVector::DotProduct(currentPosition - ControlPoints[0].PositionControlPoint, direction);

			//If the ship is past the stop point, stop it and set its target.
			if (currentDistance >= stopDistance)
			{
				StopMovementImmediately();
				SetTargetPosition(movementTimeline[0].vector);
				return true;
			}
			else
			{
				//Get the state and update its elapsed time
				FMovementState state = movementTimeline[0];
				state.elapsedTime += GetWorld()->GetDeltaSeconds();

				//Check if it is stalling
				if (state.elapsedTime >= Duration)
				{
					//If it is, Force it to go through
					UE_LOG(LogTemp, Warning, TEXT("Movement Stop Command Stalled! Forcing It To Prevent Stalling!"));
					StopMovementImmediately();
					SetTargetPosition(movementTimeline[0].vector);
					return true;
				}
				else
				{
					//Set the new state so that the timer increments
					movementTimeline[0] = state;
				}

				//Wait a tick
				return false;
			}
		}
	}
	
	//On client, stop immediately.
	//isAllowed is only true if its the host or AI
	if(!isServer || isAllowed)
	{
		StopMovementImmediately();
		SetTargetPosition(currentPosition);
		
		return true;
	}

	//This should never run
	UE_LOG(LogTemp, Error, TEXT("Stop function reached the end of the function call. An unknown error has occurred."));
	return false;
}

bool UTTCMovementComponent::StopRotation(bool isServer)
{
	bool isAllowed = false;

	//On server, continue rotation to the stop rotation.
	if (isServer)
	{
		//Check that we are NOT THE HOST OR AI!
		APawn* owner = Cast<APawn>(GetOwner());

		if (!owner)
		{
			//Failed to get pawn, throw out the command
			UE_LOG(LogTemp, Error, TEXT("Stop Command failed to cast to APawn! The rotation will be thrown out!"));
			RemoveNextFromTimeline();
			return false;
		}

		//Check that we are NOT THE HOST OR AI!
		if (owner->GetController()->IsLocalPlayerController() || !owner->IsPlayerControlled())
		{
			//This is the host or AI
			isAllowed = true;
		}
		else
		{
			//This is not the host nor AI
			//If it isn't verified we need to verify it
			if (!GetNextFromTimeline().isVerified)
			{
				//Validate that the turn stop is between the prior rotation and target rotation
				//Clockwise
				if (priorRotation.GetNormalized().Yaw <= targetRotation.GetNormalized().Yaw)
				{
					if (movementTimeline[0].rotation.GetNormalized().Yaw >= priorRotation.GetNormalized().Yaw && movementTimeline[0].rotation.GetNormalized().Yaw <= targetRotation.GetNormalized().Yaw)
					{
						//Verified to be between these values
						movementTimeline[0] = FMovementState{ movementTimeline[0].rotation, true, true, true };
					}
					else
					{
						//Verification failed. Throw out the value.
						UE_LOG(LogTemp, Error, TEXT("Stop Command had an invalid rotation! The rotation will be thrown out! prior < target"));
						RemoveNextFromTimeline();
						return false;
					}
				}
				else //Counter clockwise
				{
					if (movementTimeline[0].rotation.GetNormalized().Yaw >= priorRotation.GetNormalized().Yaw || movementTimeline[0].rotation.GetNormalized().Yaw <= targetRotation.GetNormalized().Yaw)
					{
						//Verified to be between these values
						movementTimeline[0] = FMovementState{ movementTimeline[0].rotation, true, true, false };
					}
					else
					{
						//Verification failed. Throw out the value.
						UE_LOG(LogTemp, Error, TEXT("Stop Command had an invalid rotation! The rotation will be thrown out! target < prior"));
						RemoveNextFromTimeline();
						return false;
					}
				}
			}

			//Now we stop the ship if it passes the stop point
			float delta = movementTimeline[0].rotation.GetNormalized().Yaw - meshRef->GetRelativeRotation().GetNormalized().Yaw;

			//If the ship is past the stop point, stop it and set its target.
			if (movementTimeline[0].isClockwise && delta < 0) //Clockwise
			{
				interruptRotation = true;
				SetTargetRotation(movementTimeline[0].rotation);
				return true;
			}
			else if(!movementTimeline[0].isClockwise && delta > 0) //Counter Clockwise
			{
				interruptRotation = true;
				SetTargetRotation(movementTimeline[0].rotation);
				return true;
			}
			else //Ship is not past the point yet
			{
				//Get the state and update its elapsed time
				FMovementState state = movementTimeline[0];
				state.elapsedTime += GetWorld()->GetDeltaSeconds();

				//Check if it is stalling
				if (state.elapsedTime >= Duration)
				{
					//If it is, Force it to go through
					UE_LOG(LogTemp, Warning, TEXT("Rotation Stop Command Stalled! Forcing It To Prevent Stalling!"));
					interruptRotation = true;
					SetTargetRotation(movementTimeline[0].rotation);
					return true;
				}
				else
				{
					//Set the new state so that the timer increments
					movementTimeline[0] = state;
				}

				//Wait a tick
				return false;
			}
		}
	}

	//On client, stop immediately.
	//isAllowed is only true if its the host or AI
	if (!isServer || isAllowed)
	{
		interruptRotation = true;
		SetTargetRotation(meshRef->GetRelativeRotation());
		return true;
	}

	//This should never run
	UE_LOG(LogTemp, Error, TEXT("RotateStop function reached the end of the function call. An unknown error has occurred."));
	return false;
}

void UTTCMovementComponent::DrawDebugLines(FVector currentPosition)
{
	if (GetTargetPosition() != currentPosition && debugMovement)
	{
		//Draw movement line
		DrawDebugLine(GetWorld(), ControlPoints[0].PositionControlPoint, ControlPoints[1].PositionControlPoint, FColor::Green, false, 5);

		if (GetNextFromTimeline().isStop)
		{
			//Draw a stop point for stop commands
			DrawDebugPoint(GetWorld(), GetNextFromTimeline().vector, 10, FColor::Red, false, 5);
		}
	}
}

void UTTCMovementComponent::InitializeFromController(bool useReplication, bool canRun)
{
    //Whether this instance will use the replicated position. Client Owned pawn should not.
	useReplicatedPosition = useReplication;

	//Sets the starting bool for the timeline run function. If starting value is false, run is effectively disabled. Set to false on clients, host, and ai.
	awaitTimeline = canRun;
}

void UTTCMovementComponent::InitializeFromShip(FVector currentPosition, UStaticMeshComponent* meshReference)
{
	//Set a reference to the actors mesh component
	meshRef = meshReference;

	//Set starting rotation and position
	SetTargetPosition(currentPosition);
	SetTargetRotation(meshRef->GetRelativeRotation());
}

void UTTCMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTTCMovementComponent, replicatedPosition);
	DOREPLIFETIME(UTTCMovementComponent, replicatedRotation);
}

