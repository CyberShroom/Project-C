// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "SDIO_Item.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (ShortToolTip = "Item class for the inventory system."))
class SDIO_INVENTORY_API USDIO_Item : public UObject
{
	GENERATED_BODY()

protected:
	/// <summary>
	/// Material reference for initialization
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Reference to the material to use."))
	UMaterialInterface* OutlineMaterial;

public:
	/// <summary>
	/// The name of the item
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Name of the item."))
	FText name;

	/// <summary>
	/// The sprite of the item
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Sprite of the item."))
	UTexture2D* sprite = nullptr;
	
	/// <summary>
	/// The id of the item itself
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "ID of the item."))
	FString itemID;

	/// <summary>
	/// The unique identifier of this instance of the item
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Internal Information", meta = (Tooltip = "Unqiue identifier of this instance of the item."))
	FGuid instanceID;

	/// <summary>
	/// The material instance generated at runtime. Null if no OutlineMaterial is set.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information", meta = (Tooltip = "The material instance of the items sprite."))
	UMaterialInstanceDynamic* material;

	/// <summary>
	///
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "SDIO_Inventory | Initializers")
	virtual void Initialize();
};

//////////////////////////////////////////////////////////
// Create an inventory object							//
// Create a ui inventory object							//
// Both objects need to know the existence of the other	//
// Inventory is stored in player controller				//
// UI_Inventory is stored in the inventory ui			//
//////////////////////////////////////////////////////////
