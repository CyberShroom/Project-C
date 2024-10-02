// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Window.h"
#include "SDIO_UI_TextButton.h"
#include "SDIO_UIWindow_Multiplayer.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API USDIO_UIWindow_Multiplayer : public UUI_Window
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Host;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Join;

public:
	virtual void NativeConstruct() override;
};
