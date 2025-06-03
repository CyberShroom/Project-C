// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "SDIO_Item.h"
#include "TTC_Item.generated.h"

//A list of enums that refer to the different item rarities
UENUM(BlueprintType, Category = "TTC | Enums", meta = (Tooltip = "A list of enums that refer to the different item rarities."))
enum class ERarity : uint8
{
	COMMON,
	UNCOMMON,
	RARE,
	EPIC,
	LEGENDARY,
	FUSION,
	CORRUPT,
	ARTIFACT
};

/**
 * 
 */
UCLASS(Blueprintable)
class TTC_INVENTORY_API UTTC_Item : public USDIO_Item
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Rarity of the item
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (Tooltip = "Rarity of the item."))
	ERarity rarity;

	/// <summary>
	/// Returns an FColor corresponding to a rarity.
	/// </summary>
	/// <param name="item">the rarity enum</param>
	/// <returns>a corresponding color</returns>
	UFUNCTION(BlueprintCallable, Category = "TTC_Inventory", meta = (Tooltip = "Returns an FColor corresponding to a rarity."))
	static FLinearColor GetRarityColor(ERarity item);

	virtual void Initialize() override;
};
