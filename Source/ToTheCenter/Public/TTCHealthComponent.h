// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTCHealthComponent.generated.h"


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
	UFUNCTION()
	float Damage(float amount, float& pool);

	/// <summary>
	/// Heal a health state
	/// </summary>
	/// <param name="amount">amount of healing to deal</param>
	/// <param name="pool">health pool to draw from</param>
	/// <param name="max">max size of the pool</param>
	UFUNCTION()
	float Heal(float amount, float& pool, float& max);
};
