// Fill out your copyright notice in the Description page of Project Settings.


#include "TTCHealthComponent.h"

// Sets default values for this component's properties
UTTCHealthComponent::UTTCHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UTTCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTTCHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UTTCHealthComponent::Damage(float amount, float& pool)
{
	//amount cannot be less than 0, that would be healing
	if (amount < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f Damage Dealt is less than 0. Damage instance ignored."), amount);
		return 0;
	}
	
	//Health cannot go below 0
	if (pool - amount < 0)
	{
		amount -= (amount - pool);
	}

	//Deal damage
	pool -= amount;

	//Return damage dealt
	return amount;
}

float UTTCHealthComponent::Heal(float amount, float& pool, float& max)
{
	//amount cannot be less than 0, that would be damage
	if (amount < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f Healing is less than 0. Heal instance ignored."), amount);
		return 0;
	}

	//Health cannot go above max
	if (pool + amount > max)
	{
		amount -= (pool + amount - max);
	}

	//Deal healing
	pool += amount;

	//Return healing
	return amount;
}

