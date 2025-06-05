// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemRegistry.h"

void UItemRegistry::InitializeRegistries()
{
	//Asset manager
	UAssetManager& manager = UAssetManager::Get();

	//Gets all assets in the directory
	TArray<FPrimaryAssetId> assetIDs;
	manager.GetPrimaryAssetIdList(FPrimaryAssetType("Item"), assetIDs);

	//For each asset
	for (const FPrimaryAssetId& assetID : assetIDs)
	{
		//Data of the asset
		FAssetData assetData;

		//Attempts to fetch the data of the asset
		if (manager.GetPrimaryAssetData(assetID, assetData))
		{
			//Loads the asset into memory
			UObject* loadedAsset = assetData.GetAsset();
			
			//Attempts to cast the asset into a bp
			if (UBlueprint* bp = Cast<UBlueprint>(loadedAsset))
			{
				//Loads the class into memory
				UClass* loadedClass = bp->GeneratedClass;

				//Checks that the class exists and is a TTC Item
				if (loadedClass && loadedClass->IsChildOf(UTTC_Item::StaticClass()))
				{
					//Instantiate the item
					UTTC_Item* item = NewObject<UTTC_Item>(GetTransientPackage(), loadedClass);

					//On success, add it to the registry
					if (item)
					{
						itemRegistry.Add(item);
		
						UE_LOG(LogTemp, Log, TEXT("Loaded item: %s"), *item->GetName());
					}
				}
			}
		}
	}
}

UTTC_Item* UItemRegistry::GetRandomItem()
{
	//Generate a random number
	int random = FMath::RandRange(0, itemRegistry.Num() - 1);

	//Duplicate the item in the registry
	UTTC_Item* newItem = NewObject<UTTC_Item>(GetTransientPackage(), itemRegistry[random]->GetClass());
	newItem->Initialize();
	return newItem;
}

UTTC_Item* UItemRegistry::GetItemFromID(FString id)
{
	//Search the registry for the item with the id
	for (UTTC_Item* e : itemRegistry)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *e->itemID);
		if (e->itemID.Equals(id))
		{
			//Duplicate the item in the registry
			UTTC_Item* newItem = NewObject<UTTC_Item>(GetTransientPackage(), e->GetClass());
			newItem->Initialize();
			return newItem;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("An item with the id %s was not found in the registry."), *id);
	return nullptr;
}

FString UItemRegistry::GetRandomItemID()
{
	//Generate a random number
	int random = FMath::RandRange(0, itemRegistry.Num() - 1);

	//Return the items id
	return itemRegistry[random]->itemID;
}

void UItemRegistry::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("ItemRegistrySubsystem::Initializing!"));

	InitializeRegistries();
}

void UItemRegistry::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemRegistrySubsystem: Deinitialized"));
	Super::Deinitialize();
}

void UItemRegistry::BeginDestroy()
{
	UE_LOG(LogTemp, Warning, TEXT("ItemRegistrySubsystem: BeginDestroy called"));
	Super::BeginDestroy();
}
