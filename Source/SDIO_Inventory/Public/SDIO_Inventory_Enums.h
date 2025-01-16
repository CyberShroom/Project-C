// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "SDIO_Inventory", meta = (Tooltip = "Used to identify the kind of slot type."))
enum class ESlotType : uint8 
{
	Passive,
	Active,
	Weapon,
	Projectile,
	Equipment,
	Other,

	Universal
};

UENUM(BlueprintType, Category = "SDIO_Inventory", meta = (Tooltip = "Inventory ID."))
enum class EInventoryID : uint8
{
	NOID,
	Main_Inventory,
	Hotbar_Inventory,
	Equipment_Inventory,
	Passive_Inventory
};
