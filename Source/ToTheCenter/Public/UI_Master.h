// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Master.generated.h"

/**
 * 
 */
UCLASS(meta=(ShortToolTip = "Contains Functions For All Classes Within SDIO_UI"))
class TOTHECENTER_API UUI_Master : public UUserWidget
{
	GENERATED_BODY()
	
	protected:

	UFUNCTION(BlueprintCallable,Category="SDIO_UI",meta=(Tooltip="Sets the first widget to hidden and the second widget to visible"))
	void SwapUIFocus(UPARAM(ref) UWidget*& oldWidget, UPARAM(ref) UWidget*& newWidget);
};
