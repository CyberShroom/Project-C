// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType, Category = "SDIO_Items", meta = (Tooltip = "Used to identify the kind of slot type."))
enum class ESlotType : uint8 
{
	Passive,
	Active,
	Weapon,
	Projectile,
	Other,

	Universal
};
