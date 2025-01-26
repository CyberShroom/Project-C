// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LocalSettingsSave.h"
#include "Kismet/GameplayStatics.h"
#include "TTCGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API UTTCGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FString settingsSlot = "Settings";

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TTC | Settings")
	ULocalSettingsSave* settingsSave;

	UFUNCTION(BlueprintCallable, Category = "TTC | Settings")
	void SaveSettings();

	virtual void Init() override;
	virtual void Shutdown() override;
};
