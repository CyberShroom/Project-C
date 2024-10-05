// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TTC_Item_Template.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TOTHECENTER_API FTTC_Item_Template
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FText name;
	UPROPERTY()
	UTexture2D* sprite;
	//UPROPERTY()
	//TTC_Item_Type type;
	UPROPERTY()
	FGuid itemID;
};
