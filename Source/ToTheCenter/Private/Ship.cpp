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

	healthComponent = CreateDefaultSubobject<UTTCHealthComponent>(TEXT("HealthComponent"));

	movementComponent->AddControlPointPosition(FVector::ZeroVector, true);
	movementComponent->AddControlPointPosition(FVector::ZeroVector, true);
	movementComponent->FinaliseControlPoints();
}

//Damage order is Shield --> Armor --> Hull
void AShip::ShipTakeDamage(float damage)
{
	//Server only
	if (HasAuthority())
	{
		//If shield is active, damage the shield
		if (currentShield > 0)
		{
			//Damage shields
			float result = healthComponent->Damage(damage, currentShield);
			MulticastRPC_NotifyHealthChange(-result, currentShield, EHealthPools::SHIELD);
		}
		else if (armor > 0)//No? well if armor is acitve, damage the armor
		{
			//Damage armor
			float result = healthComponent->Damage(damage, armor);
			MulticastRPC_NotifyHealthChange(-result, armor, EHealthPools::ARMOR);
		}
		else
		{
			//Damage hull
			float result = healthComponent->Damage(damage, currentHull);
			MulticastRPC_NotifyHealthChange(-result, currentHull, EHealthPools::HULL);
		}

		//reset combat timer
		outOfCombatTimer = 5;
	}
}

void AShip::HealHull(float heal)
{
	//Server only
	if (HasAuthority())
	{
		//heal the hull
		float result = healthComponent->Heal(heal, currentHull, maxHull);
		MulticastRPC_NotifyHealthChange(result, currentHull, EHealthPools::HULL);
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

void AShip::MulticastRPC_NotifyHealthChange_Implementation(float amount, float newValue, EHealthPools pool)
{
	//Set values based on the servers given value.
	switch (pool)
	{
		case EHealthPools::SHIELD:

			currentShield = newValue;

			//then its damage
			if (amount < 0)
			{
				onShieldDamage.Broadcast(currentShield, -amount);
			}

			//Shield is gone
			if (newValue == 0)
			{
				onShieldBreak.Broadcast();
			}

			break;
		case EHealthPools::ARMOR:

			armor = newValue;

			//then its damage
			if (amount < 0)
			{
				onArmorDamage.Broadcast(armor, -amount);
			}
			else //then its healing
			{
				onGainArmor.Broadcast(armor, amount);
			}

			//Armor is gone
			if (newValue == 0)
			{
				onArmorBreak.Broadcast();
			}

			break;
		case EHealthPools::HULL:

			currentHull = newValue;

			//then its damage
			if (amount < 0)
			{
				onHullDamage.Broadcast(currentHull, -amount);
			}
			else //then its healing
			{
				onHullHeal.Broadcast(currentHull, amount);
			}

			break;
	}
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

	movementComponent->Duration = movementDuration;
	movementComponent->InitializeFromShip(GetActorLocation(), shipMesh);

	shipInventory = NewObject<UInventory>();
	shipInventory->Initialize(8, currentHotbarSize * 2, EInventoryID::Hotbar_Inventory);

	SetActorTickEnabled(true);
}

bool AShip::MoveShip(float joystickValue)
{
	if (HasAuthority() && !GetController()->IsLocalPlayerController() && GetController()->IsPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid call to MoveShip from a Ship Actor on the server. MoveShip can only be called by the host and AI on the server."));
		return false;
	}

	//Create a new forward vector and move the ship
	return movementComponent->Move(CalculateMovementVector(joystickValue, GetCorrectedForwardVector()), GetActorLocation(), HasAuthority());
}

void AShip::AddVectorToTimeline(FVector forwardVector, bool isStop)
{
	//Only allow server to add forward vectors to the timeline. If isStop is true, the vector is a stop vector instead.
	if (HasAuthority())
	{
		movementComponent->AddVectorToTimeline(forwardVector, isStop);
	}
}

void AShip::AddRotationToTimeline(FRotator rotation, bool isStop)
{
	//Only allow server to add rotation to the timeline. If isStop is true, the rotation is a stop rotation instead.
	if (HasAuthority())
	{
		movementComponent->AddRotationToTimeline(rotation, isStop);
	}
}

bool AShip::TurnShip(float joystickValue)
{
	//If is server, not host, and not ai
	if (HasAuthority() && !GetController()->IsLocalPlayerController() && GetController()->IsPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid call to TurnShip from a Ship Actor on the server. TurnShip can only be called by the host and AI on the server."));
		return false;
	}

	//Create a new rotation and turn the ship
	return movementComponent->Turn(CalculateRotation(joystickValue), HasAuthority());
}

void AShip::StopShip()
{
	//If is server, not host, and not AI
	if (HasAuthority() && !GetController()->IsLocalPlayerController() && GetController()->IsPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid call to StopShip from a Ship Actor on the server. StopShip can only be called by the host and AI on the server."));
		return;
	}

	//Clients and AI always stop immediatly
	movementComponent->StopMovement(GetActorLocation(), HasAuthority());
	SetActorLocation(movementComponent->GetTargetPosition());
}

void AShip::StopShipRotation()
{
	if (HasAuthority() && !GetController()->IsLocalPlayerController() && !GetController()->IsPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid call to StopShipRotation from a Ship Actor on the server. StopShipRotation can only be called by the host and AI on the server."));
		return;
	}

	movementComponent->StopRotation(HasAuthority());
	shipMesh->SetRelativeRotation(movementComponent->GetTargetRotation());
}

FVector AShip::GetCorrectedForwardVector()
{
	return FVector(shipMesh->GetForwardVector().Y, shipMesh->GetForwardVector().X * -1, shipMesh->GetForwardVector().Z);
}

FVector AShip::CalculateMovementVector(float joystickValue, FVector vector)
{
	return vector * (moveSpeed * 100 * joystickValue);
}

FRotator AShip::CalculateRotation(float joystickValue)
{
	return FRotator(0, turnSpeed * 10 * joystickValue, 0);
}

void AShip::Initialize(bool useReplication, bool canRun)
{
	movementComponent->InitializeFromController(useReplication, canRun);
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

	//If movement debug is enabled, this will draw debug lines.
	movementComponent->DrawDebugLines(GetActorLocation());
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
}

