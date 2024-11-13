// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SDIO_UIManager_Game.h"
#include "Base_HUD.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API ABase_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	/// <summary>
	/// Reference to the UI manger of this hud.
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information", meta = (Tooltip = "Reference to the UI manger of this hud."))
	USDIO_UIManager_Game* uiManagerRef;
};
