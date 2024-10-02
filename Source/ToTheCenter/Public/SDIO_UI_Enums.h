// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SDIO_UI_Enums.generated.h"

UENUM(BlueprintType, Category = "SDIO_UI", meta=(Tooltip="Used by the UI_Manager to identify a UI_Window"))
enum class EUIID : uint8
{
	TitleUI,
	MultiplayerUI,
	OptionsUI,
	LobbyUI,
	InventoryUI,

	NoID
};