// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "CargoShip.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API ACargoShip : public AShip
{
	GENERATED_BODY()
	
private:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UStaticMeshComponent* cargoMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UWidgetComponent* interactWidget;

public:
	ACargoShip();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
