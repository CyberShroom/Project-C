// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Master.h"
#include "SDIO_UI_Enums.h"
#include "UI_Window.generated.h"


class UUI_Manager; //Forward Declaration
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetCurrentUI, EUIID, UIID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSetCurrentUIToDefault);

UCLASS()
class TOTHECENTER_API UUI_Window : public UUI_Master
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Internal Information", meta = (Tooltip = "This UI_Window's UI_Manager."))
	UUI_Manager* managerReference;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "INSTANCE DEPENDANT", meta = (Tooltip = "The UIID of the UI_Window."))
	EUIID UIID = EUIID::NoID;
public:
	UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Event Dispatchers", meta = (Tooltip = "An event that, when called, sets the UI_Managers currentUI to the given UI."))
	FSetCurrentUI OnSetCurrentUI;

	UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Event Dispatchers", meta = (Tooltip = "An event that, when called, sets the UI_Managers currentUI to it's defaultUI."))
	FSetCurrentUIToDefault OnSetCurrentUIToDefault;

	UFUNCTION(BlueprintPure, Category = "SDIO_UI", meta = (Tooltip = "Returns the value of UIID."))
	EUIID getUIID();
};
