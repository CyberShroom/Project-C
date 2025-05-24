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
	DOREPLIFETIME(AShip, maxHull)
	DOREPLIFETIME(AShip, currentHull);
	DOREPLIFETIME(AShip, armor);
	DOREPLIFETIME(AShip, maxShield);
	DOREPLIFETIME(AShip, currentShield);
}

