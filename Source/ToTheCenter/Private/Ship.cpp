// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(scene);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(scene);
	camera->bAutoActivate = false;

	shipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	shipMesh->SetupAttachment(scene);

	movementComponent = CreateDefaultSubobject<UTTCMovementComponent>(TEXT("MovementComponent"));
	movementComponent->Duration = movementDuration;
	movementComponent->TeleportType = ETeleportType::ResetPhysics;
	movementComponent->bConstrainToPlane = true;
	movementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);

	movementComponent->AddControlPointPosition(FVector::ZeroVector, true);
	movementComponent->AddControlPointPosition(FVector::ZeroVector, true);
	movementComponent->FinaliseControlPoints();
}

//Damage order is Shield --> Armor --> Hull
void AShip::ShipTakeDamage(float damage)
{
	//Dont run this if there's no damage
	if (damage == 0)
	{
		return;
	}

	if (HasAuthority())
	{
		//If shield is active, damage the shield
		if (currentShield > 0)
		{
			//If damage would go over shield, nullify extra damage
			if (currentShield - damage < 0)
			{
				damage = currentShield;
				onShieldBreak.Broadcast();
			}

			//Deal damage to the shield
			currentShield -= damage;

			//Run on shield damage items
			onShieldDamage.Broadcast(currentShield, damage);

			//Do not run rpc if host
			if (GetController()->IsLocalPlayerController() == false)
			{
				//Notify client that they took damage so client can run cosmetic changes.
				ClientRPC_NotifyClientOfShieldChange(damage);
			}
		}
		else if (armor > 0)
		{
			//If damage would go over armor, nullify extra damage
			if (armor - damage < 0)
			{
				damage = armor;
				onArmorBreak.Broadcast();
			}

			//Deal damage to the shield
			armor -= damage;

			//Run on shield damage items
			onArmorDamage.Broadcast(armor, damage);

			//Do not run rpc if host
			if (GetController()->IsLocalPlayerController() == false)
			{
				//Notify client that they took damage so client can run cosmetic changes.
				ClientRPC_NotifyClientOfArmorChange(true, damage);
			}
		}
		else
		{
			//Deal damage to hull
			currentHull -= damage;

			//Run on damage items
			onHullDamage.Broadcast(currentHull, damage);

			//Do not run rpc if host
			if (GetController()->IsLocalPlayerController() == false)
			{
				//Notify client that they took damage so client can run cosmetic changes.
				ClientRPC_NotifyClientOfHullChange(true, damage);
			}
		}

		outOfCombatTimer = 5;
	}
}

void AShip::HealHull(float heal)
{
	if (HasAuthority())
	{
		//If health is max, do nothing
		if (currentHull > maxHull)
		{
			return;
		}

		//Ensure health does not go above the max
		if (currentHull + heal > maxHull)
		{
			heal = maxHull - currentHull;
			currentHull = maxHull;
		}
		else
		{
			currentHull += heal;
		}

		//Run on heal items
		onHullHeal.Broadcast(currentHull, heal);
		
		//Do not run rpc if host
		if (GetController()->IsLocalPlayerController() == false)
		{
			//Notify client that they healed so client can run cosmetic changes.
			ClientRPC_NotifyClientOfHullChange(false, heal);
		}
	}
}

float AShip::GetMaxHull()
{
	return maxHull;
}

float AShip::GetMaxShield()
{
	return maxShield;
}

float AShip::GetArmor()
{
	return armor;
}

FRotator AShip::GetTargetRotation()
{
	return targetRotation;
}

float AShip::GetMovementDuration()
{
	return movementDuration;
}

void AShip::OnRep_maxHull()
{
	onMaxHullChanged.Broadcast(maxHull);
}

void AShip::OnRep_currentHull()
{
	//Do 0 because on damage effects will ignore 0's preferably
	onHullDamage.Broadcast(currentHull, 0);
}

void AShip::OnRep_maxShield()
{
	onMaxShieldChanged.Broadcast(maxShield);
}

void AShip::OnRep_currentShield()
{
	//Do 0 because on damage effects will ignore 0's preferably
	onShieldDamage.Broadcast(currentShield, 0);
}

void AShip::OnRep_armor()
{
	onArmorDamage.Broadcast(armor, 0);
}

void AShip::OnRep_targetRotation()
{
	if (!GetController())
	{
		Rotate(targetRotation, 0);
	}
}

void AShip::ClientRPC_NotifyClientOfShieldChange_Implementation(float amount)
{
	if (currentShield - amount == 0)
	{
		onShieldBreak.Broadcast();
	}

	onShieldDamage.Broadcast(currentShield, amount);
}

void AShip::ClientRPC_NotifyClientOfArmorChange_Implementation(bool isDamage, float amount)
{
	if (armor - amount == 0)
	{
		onArmorBreak.Broadcast();
	}

	if (isDamage)
	{
		onArmorDamage.Broadcast(armor, amount);
	}
	else
	{
		onGainArmor.Broadcast(armor, amount);
	}
}

void AShip::Rotate(FRotator newRotation, float elapsedTime)
{
	//Stop rotation if the flag is true
	if (bStopRotation)
	{
		return;
	}

	//Keep track of time
	elapsedTime += GetWorld()->GetDeltaSeconds();

	//Increment the rotation
	shipMesh->SetRelativeRotation(FMath::RInterpConstantTo(shipMesh->GetRelativeRotation(), newRotation, GetWorld()->GetDeltaSeconds(), rotationSpeed));

	//Continue on next tick or set the rotation when done
	if (elapsedTime >= movementDuration)
	{
		shipMesh->SetRelativeRotation(newRotation);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, newRotation, elapsedTime]() {Rotate(newRotation, elapsedTime); });
	}
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	targetRotation = shipMesh->GetRelativeRotation();
	lastPredictedRotation = shipMesh->GetRelativeRotation();
	
	movementComponent->Duration = movementDuration;
	movementComponent->Initialize(GetActorLocation());

	shipInventory = NewObject<UInventory>();
	shipInventory->Initialize(8, currentHotbarSize * 2, EInventoryID::Hotbar_Inventory);

	SetActorTickEnabled(true);
}

bool AShip::MoveShip(float joystickValue)
{
	//IF SERVER AND NOT HOST AND PLAYER CONTROLLED
	if (HasAuthority() && !GetController()->IsLocalPlayerController() && IsPlayerControlled())
	{
		//Use the timeline and move the ship
		bool result = movementComponent->Move(movementComponent->GetNextFromTimeline(), GetActorLocation(), HasAuthority());

		//On success, remove the vector from the timeline
		if (result)
		{
			movementComponent->RemoveNextFromTimeline();
		}

		return result;
	}
	else
	{
		//Create a new forward vector and move the ship
		return movementComponent->Move(CalculateMovementForwardVector(joystickValue), GetActorLocation(), HasAuthority());
	}
}

void AShip::AddVectorToTimeline(FVector forwardVector)
{
	//Only allow server to add forward vectors to the timeline
	if (HasAuthority())
	{
		movementComponent->AddToTimeline(forwardVector);
	}
}

bool AShip::TurnShip(float joystickValue, bool useTarget, FRotator& predictedRotation)
{
	//Normalized rotation values for comparison and checking for errrors
	float normalizedShipYaw = FMath::Fmod(shipMesh->GetRelativeRotation().Yaw + 180.0f, 360.0f) - 180.0f;
	float normalizedPredictedYaw = FMath::Fmod(lastPredictedRotation.Yaw + 180.0f, 360.0f) - 180.0f;

	//Normalize ship yaw values
	if (normalizedShipYaw < -180)
	{
		normalizedShipYaw += 360;
	}
	else if (normalizedShipYaw > 180)
	{
		normalizedShipYaw -= 360;
	}

	//Normalize predicted yaw values
	if (normalizedPredictedYaw < -180)
	{
		normalizedPredictedYaw += 360;
	}
	else if (normalizedPredictedYaw > 180)
	{
		normalizedPredictedYaw -= 360;
	}

	//If using relative rotation and ship Yaw is not at the target yaw, don't rotate until it matches.
	if (FMath::IsNearlyEqual(normalizedShipYaw, normalizedPredictedYaw) == false && useTarget == false && bStopRotation == false)
	{
		//Check for edge case. If either edge case is true, do not return false. If neither edge case is true, return false.
		if (!(normalizedShipYaw == 180.0f && normalizedPredictedYaw == -180.0f) && !(normalizedShipYaw == -180.0f && normalizedPredictedYaw == 180.0f))
		{
			return false;
		}
	}

	//If true, reset the value
	if (bStopRotation)
	{
		bStopRotation = false;
	}

	//The rotation to set
	FRotator finalRotation;

	//Get the rotation to go to
	if (useTarget)
	{
		finalRotation = targetRotation + FRotator(0, turnSpeed * 10 * joystickValue, 0);
	}
	else
	{
		finalRotation = shipMesh->GetRelativeRotation() + FRotator(0, turnSpeed * 10 * joystickValue, 0);
	}

	//Add the offset
	finalRotation + rotationOffset;
	rotationOffset = FRotator(0, 0, 0);

	//Set target if server
	if (HasAuthority())
	{
		targetRotation = finalRotation;
	}
	else
	{
		predictedRotation = finalRotation;
	}
	lastPredictedRotation = finalRotation;

	//Begin rotating
	rotationSpeed = FMath::Abs(shipMesh->GetRelativeRotation().Yaw - finalRotation.Yaw) / movementDuration;
	Rotate(finalRotation, 0);

	return true;

	//shipMesh->AddLocalRotation(FRotator(0.0, turnSpeed * 75 * joystickValue, 0.0));
}

void AShip::StopShip(FVector& predictedLocation)
{
	//Stop movement immediately
	movementComponent->StopMovementImmediately();

	if (HasAuthority())
	{
		if (IsLocallyControlled() == false)
		{
			//Validate that argument is valid
			FVector direction = (movementComponent->ControlPoints[1].PositionControlPoint - GetActorLocation()).GetSafeNormal();
			float length = FVector::Dist(GetActorLocation(), movementComponent->ControlPoints[1].PositionControlPoint);

			float projectedLength = FVector::DotProduct(predictedLocation - GetActorLocation(), direction);

			if (projectedLength >= 0 && projectedLength <= length)
			{
				SetActorLocation(predictedLocation);
			}
			else
			{
				movementComponent->RestartMovement();
				return;
			}
		}

		//targetLocation = GetActorLocation();
	}
	else
	{
		predictedLocation = GetActorLocation();
	}

	//Add control points to prevent array out of bound error
	movementComponent->ResetControlPoints();
	movementComponent->AddControlPointPosition(FVector(0, 0, 0), true);
	movementComponent->AddControlPointPosition(GetActorLocation(), false);
	movementComponent->FinaliseControlPoints();
}

void AShip::StopShipRotation(FRotator& predictedRotation)
{
	bStopRotation = true;

	if (HasAuthority())
	{
		if (IsLocallyControlled() == false)
		{
			shipMesh->SetRelativeRotation(predictedRotation);
		}
		targetRotation = shipMesh->GetRelativeRotation();
	}
	else
	{
		predictedRotation = shipMesh->GetRelativeRotation();
	}
	lastPredictedRotation = shipMesh->GetRelativeRotation();
}

FVector AShip::GetCorrectedForwardVector()
{
	return FVector(shipMesh->GetForwardVector().Y, shipMesh->GetForwardVector().X * -1, shipMesh->GetForwardVector().Z);
}

FVector AShip::CalculateMovementForwardVector(float joystickValue)
{
	return GetCorrectedForwardVector() * (moveSpeed * 100 * joystickValue);
}

FVector AShip::GetTargetPosition()
{
	return movementComponent->GetTargetPosition();
}

void AShip::SetMovementReplication(bool value)
{
	movementComponent->useReplicatedPosition = value;
}

void AShip::ClientRPC_NotifyClientOfHullChange_Implementation(bool isDamage, float amount)
{
	if (isDamage)
	{
		onHullDamage.Broadcast(currentHull, amount);
	}
	else
	{
		onHullHeal.Broadcast(currentHull, amount);
	}
}

void AShip::ClientRPC_CheckForLocationError_Implementation(FVector trueLocation)
{
	movementComponent->CheckForDesync(trueLocation);
}

void AShip::ClientRPC_CheckForRotationError_Implementation(FRotator trueRotation, FRotator predictedRotation)
{
	if (rotationErrorCheckWait > 0)
	{
		rotationErrorCheckWait--;
		return;
	}

	rotationOffset = FRotator(0, FMath::Fmod((trueRotation.Yaw - predictedRotation.Yaw) + 180, 360) - 180, 0);

	rotationErrorCheckWait = 1 / movementDuration;
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If value changes in editor blueprint mid-test, update the duration in the component.
	if (movementComponent->Duration != movementDuration)
	{
		movementComponent->Duration = movementDuration;
	}

	if (HasAuthority())
	{
		//While in combat, do not regen shield
		if (outOfCombatTimer > 0)
		{
			shieldRegenTimer = 0;
			outOfCombatTimer -= DeltaTime;
		}
		else if (currentShield < maxShield) //Only regen shield if server and also shield is less than max
		{
			shieldRegenTimer += DeltaTime;

			if (shieldRegenTimer >= 1 / shieldRegenPerSecond)
			{
				shieldRegenTimer -= 1 / shieldRegenPerSecond;
				currentShield += 1;

				//If this is the host, the event must be called manually. OnRep will not run.
				if (GetController()->IsLocalPlayerController())
				{
					onShieldDamage.Broadcast(currentShield, 0);
				}
			}
		}
		else
		{
			//If nothing is happening, ensure shield timer is reset
			shieldRegenTimer = 0;
		}
	}

	//Only run on server, not the host though.
	//Make the ship automatically move if it has forward vectors in its timeline.
	if (HasAuthority() && !GetController()->IsLocalPlayerController())
	{
		//Move the ship when possible
		if (MoveShip(0))
		{
			//When successful, call check for desync
			ClientRPC_CheckForLocationError(movementComponent->GetTargetPosition());
		}
	}
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShip, moveSpeed);
	DOREPLIFETIME(AShip, turnSpeed);
	DOREPLIFETIME(AShip, maxHull)
	DOREPLIFETIME(AShip, currentHull);
	DOREPLIFETIME(AShip, armor);
	DOREPLIFETIME(AShip, maxShield);
	DOREPLIFETIME(AShip, currentShield);
	DOREPLIFETIME(AShip, targetRotation);
}

