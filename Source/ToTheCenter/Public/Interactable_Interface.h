// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Interactable_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOTHECENTER_API IInteractable_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(APlayerController* controller);

	/// <summary>
	/// Changes the alpha of a widget based on the proximity to the player
	/// </summary>
	/// <param name="actorLocation">location of the player</param>
	/// <param name="widget">widget to change the alpha of</param>
	/// <param name="worldRef">reference to world</param>
	UFUNCTION()
	virtual void CheckProx(FVector actorLocation, UWidgetComponent* widget, UWorld* worldRef);
};
