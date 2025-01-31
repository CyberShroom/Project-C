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
	
	shipInventory = NewObject<UInventory>();
	shipInventory->Initialize(8,currentHotbarSize * 2, EInventoryID::Hotbar_Inventory);
}

void AShip::MoveShip(float joystickValue)
{
	AddActorLocalOffset(FVector(moveSpeed * 200 * joystickValue, 0.0, 0.0));
}

void AShip::TurnShip(float joystickValue)
{
	AddActorLocalRotation(FRotator(0.0, turnSpeed * 75 * joystickValue, 0.0));
	camera->AddRelativeRotation(FRotator(0.0, turnSpeed * 75 * joystickValue * -1, 0.0));
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

