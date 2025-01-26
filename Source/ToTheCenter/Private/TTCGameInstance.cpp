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
}

void UTTCGameInstance::Shutdown()
{
	SaveSettings();
}


