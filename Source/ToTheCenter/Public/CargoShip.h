// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "TTC_Item.h"
#include "Interactable_Interface.h"
#include "Item_Frame.h"
#include "CargoShip.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API ACargoShip : public AShip, public IInteractable_Interface
{
	GENERATED_BODY()
	
private:
	/// <summary>
	/// The item contained in this cargo ship
	/// </summary>
	UPROPERTY()
	FString containedItem;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UStaticMeshComponent* cargoMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UWidgetComponent* interactWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FString tempItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TSubclassOf<AActor> frameRef;

public:
	ACargoShip();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(APlayerController* controller) override;
};
