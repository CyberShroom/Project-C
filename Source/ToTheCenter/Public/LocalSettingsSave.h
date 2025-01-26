// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LocalSettingsSave.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API ULocalSettingsSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TTC | Settings", meta = (Tooltip = "User prefered volume for sounds."))
	float soundVolume = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TTC | Settings", meta = (Tooltip = "User prefered volume for music."))
	float musicVolume = 1.0;

};
