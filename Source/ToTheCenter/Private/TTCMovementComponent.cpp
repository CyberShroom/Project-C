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

	//Set the interpolation values
	StopMovementImmediately();
	ResetControlPoints();
	AddControlPointPosition(FVector(0, 0, 0), true);
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
	AddControlPointPosition(FVector(0, 0, 0), true);
	AddControlPointPosition(targetPosition, false);
	FinaliseControlPoints();
	RestartMovement();
}

void UTTCMovementComponent::SetTargetPosition(FVector newPosition)
{
	targetPosition = newPosition;
	replicatedPosition = targetPosition;
}

void UTTCMovementComponent::AddToTimeline(FVector newVector)
{
	movementTimeline.Add(newVector);
}

void UTTCMovementComponent::RemoveNextFromTimeline()
{
	movementTimeline.RemoveAt(0);
	offsetIndex--;
}

FVector UTTCMovementComponent::GetNextFromTimeline()
{
	//Check if timeline is empty and return Zero if it is.
	if (movementTimeline.IsValidIndex(0))
	{
		return movementTimeline[0];
	}
	else
	{
		return FVector::Zero();
	}
}

FVector UTTCMovementComponent::GetTargetPosition()
{
	return targetPosition;
}

void UTTCMovementComponent::CheckForDesync(FVector finalLocation)
{
	if (!movementTimeline[0].Equals(finalLocation) && offsetIndex > 0)
	{
		offset = finalLocation - movementTimeline[0];
	}

	//Set the offset index equal to the index of the next position that will be added to the timeline.
	offsetIndex = movementTimeline.Num();

	RemoveNextFromTimeline();
}

void UTTCMovementComponent::Turn()
{

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

