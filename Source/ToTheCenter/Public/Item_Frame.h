// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TTC_Item.h"
#include "Item_Slot.h"
#include "Components/WidgetComponent.h"
#include "Interactable_Interface.h"
#include "Base_Player_Controller.h"
#include "ItemRegistry.h"
#include "Item_Frame.generated.h"

UCLASS()
class TOTHECENTER_API AItem_Frame : public AActor, public IInteractable_Interface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	USceneComponent* scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UWidgetComponent* widgetRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UWidgetComponent* interactWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Hierarchy References")
	UItem_Slot* slotRef;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AItem_Frame();

	/// <summary>
	/// Sets the contained item in the frame. If left null, generate a new item.
	/// </summary>
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "TTC | Multicasts")
	void Multicast_SetItem(const FString& item);

	UFUNCTION()
	bool SlotReferenceIsValid();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APlayerController* controller) override;

};
