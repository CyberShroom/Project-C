// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TTC_Item.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"
#include "ItemRegistry.generated.h"

/**
 * 
 */
UCLASS()
class TTC_INVENTORY_API UItemRegistry : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	/// <summary>
	/// Registry of all items in the game
	/// </summary>
	UPROPERTY()
	TArray<UTTC_Item*> itemRegistry;

	/// <summary>
	/// Initialize all item related registries
	/// </summary>
	UFUNCTION()
	void InitializeRegistries();

public:
	/// <summary>
	/// Returns a copy of a random item from the registry
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "TTC", meta = (Tooltip = "Returns a copy of a random item from the registry"))
	UTTC_Item* GetRandomItem();

	/// <summary>
	/// Returns a copy of an item with the given id, or nullptr if it doesn't exist
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "TTC", meta = (Tooltip = "Returns a copy of an item with the given id, or nullptr if it doesn't exist"))
	UTTC_Item* GetItemFromID(FString id);

	/// <summary>
	/// Returns the id of a random item in the registry
	/// </summary>
	UFUNCTION(BlueprintPure, Category = "TTC", meta = (Tooltip = "Returns the id of a random item in the registry"))
	FString GetRandomItemID();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
};
