// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTCHealthComponent.generated.h"

USTRUCT(BlueprintType) //Contains data on the movement.
struct FHealthPool
{
	GENERATED_USTRUCT_BODY()

	//Max size of the pool
	float max;

	//Current size of the pool
	float current;

	//Empty constructor. Should be ignored.
	FHealthPool()
	{
		max = 0;
		current = 0;
	}

	//Construct a health pool
	FHealthPool(float newMax, float newCurrent)
	{
		max = newMax;

		current = newCurrent;

		//Current cannot be above max.
		if (current > max)
		{
			current = max;
		}
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOTHECENTER_API UTTCHealthComponent : public UActorComponent
{

	GENERATED_BODY()

private:
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	UTTCHealthComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// Deal damage to a health state
	/// </summary>
	/// <param name="amount">amount of damage to deal</param>
	/// <param name="pool">health pool to draw from</param>
	UFUNCTION(BlueprintCallable, Category = "Health Component", meta = (Tooltip = "Damages a health pool."))
	float Damage(float amount, UPARAM(ref) float& pool);

	/// <summary>
	/// Heal a health state
	/// </summary>
	/// <param name="amount">amount of healing to deal</param>
	/// <param name="pool">health pool to draw from</param>
	UFUNCTION(BlueprintCallable, Category = "Health Component", meta = (Tooltip = "Heals a health pool."))
	float Heal(float amount, UPARAM(ref) FHealthPool& pool);

	/// <summary>
	/// Heal a health pool by a certain amount per second.
	/// </summary>
	/// <param name="perSec">How much health should regenerate per second.</param>
	/// <param name="pool">The health pool.</param>
	/// <param name="interupt">Value that can stop regen early. Set to a static 0 to disable interupt.</param>
	UFUNCTION(BlueprintCallable, Category = "Health Component", meta = (Tooltip = "Regenerates a health pool over time."))
	void Regen(float perSec, UPARAM(ref) FHealthPool& pool, UPARAM(ref) float& interupt);
};
