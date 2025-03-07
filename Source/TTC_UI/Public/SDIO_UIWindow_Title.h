// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Window.h"
#include "SDIO_UI_TextButton.h"
#include "SDIO_UIWindow_Title.generated.h"

/**
 * 
 */
UCLASS(meta = (ShortToolTip = "C Class. The Title Window Base Class."))
class TTC_UI_API USDIO_UIWindow_Title : public UUI_Window
{
	GENERATED_BODY()

private:
	////Event Delegates For Text_Buttons////

	/// <summary>
	/// Broadcasts Multiplayer button event
	/// </summary>
	UFUNCTION()
	void DelegateOpenMultiplayer();

	/// <summary>
	/// Broadcasts Options button event
	/// </summary>
	UFUNCTION()
	void DelegateOpenOptions();

protected:
	////Object References////
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Singleplayer;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Multiplayer;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Options;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Quit;

public:
	virtual void InitializeWindow() override;
	virtual void NativeConstruct() override;
};
