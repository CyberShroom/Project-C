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
	if (forwardVector.IsZero() || forwardVector.ContainsNaN())
	{
		return false;
	}

	//Update the target position
	SetTargetPosition(targetPosition + forwardVector + offset);
	offset = FVector::Zero();

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
		AddToTimeline(targetPosition);
	}

	return true;
}

void UTTCMovementComponent::OnRep_targetPosition()
{
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

void UTTCMovementComponent::SetTargetPosition(FVector newPosition)
{
	targetPosition = newPosition;
	replicatedPosition = targetPosition;
}

void UTTCMovementComponent::AddToTimeline(FVector newVector, bool isStop)
{
	movementTimeline.Add(FMovementState{newVector, isStop});
}

void UTTCMovementComponent::RemoveNextFromTimeline()
{
	movementTimeline.RemoveAt(0);
	offsetIndex--;
}

FMovementState UTTCMovementComponent::GetNextFromTimeline()
{
	//Check if timeline is empty and return Zero if it is.
	if (movementTimeline.IsValidIndex(0))
	{
		return movementTimeline[0];
	}
	else
	{
		return FMovementState{FVector::Zero(), false};
	}
}

FVector UTTCMovementComponent::GetTargetPosition()
{
	return targetPosition;
}

void UTTCMovementComponent::CheckForDesync(FVector finalLocation)
{
	if (!movementTimeline[0].vector.Equals(finalLocation) && offsetIndex > 0)
	{
		offset = finalLocation - movementTimeline[0].vector;
		UE_LOG(LogTemp, Error, TEXT("Desync Detected! Applying Offset: %f %f %f to compensate."), offset.X, offset.Y, offset.Z);
	}

	//Set the offset index equal to the index of the next position that will be added to the timeline.
	offsetIndex = movementTimeline.Num();

	RemoveNextFromTimeline();
}

void UTTCMovementComponent::Turn()
{

}

bool UTTCMovementComponent::Stop(FVector currentPosition, bool isServer)
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
			float stopDistance = FVector::DotProduct(ControlPoints[1].PositionControlPoint - ControlPoints[0].PositionControlPoint, direction);
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

void UTTCMovementComponent::Initialize(FVector originalPosition)
{
	targetPosition = originalPosition;
}

void UTTCMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTTCMovementComponent, replicatedPosition);
}

