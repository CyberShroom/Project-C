// Fill out your copyright notice in the Description page of Project Settings.


#include "TTCGameInstance.h"

void UTTCGameInstance::SaveSettings()
{
	bool isSuccessful = UGameplayStatics::SaveGameToSlot(settingsSave, settingsSlot, 0);

	if (isSuccessful == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed To Save Settings To File."));
	}
}

void UTTCGameInstance::Init()
{
	Super::Init();

	settingsSave = NewObject<ULocalSettingsSave>();

	ULocalSettingsSave* tempSave = Cast<ULocalSettingsSave>(UGameplayStatics::LoadGameFromSlot(settingsSlot, 0));

	if (tempSave)
	{
		settingsSave = tempSave;
	}
	else
	{
		bool isSuccessful = UGameplayStatics::SaveGameToSlot(settingsSave, settingsSlot, 0);

		if (isSuccessful == false)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed To Create Settings Save File."));
		}
	}

	//Initialize item registry
	FAssetRegistryModule& assetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	//Check if the asset registry is still loading
	if (assetRegistry.Get().IsLoadingAssets())
	{
		//Assets not loaded, create a lambda for when they are
		assetRegistry.Get().OnFilesLoaded().AddLambda([this]()
		{
			UItemRegistry* registry = GetSubsystem<UItemRegistry>();
		});
	}
	else
	{
		//Assets loaded, initialize item registry
		UItemRegistry* registry = GetSubsystem<UItemRegistry>();
	}
}

void UTTCGameInstance::Shutdown()
{
	SaveSettings();
}


