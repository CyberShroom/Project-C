// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Window_BaseOptions.h"
#include "SDIO_UI_TextButton.h"
#include "SDIO_UIWindow_Options.generated.h"

/**
 * 
 */
UCLASS(meta = (ShortToolTip = "Options window base class."))
class TTC_UI_API USDIO_UIWindow_Options : public UUI_Window_BaseOptions
{
	GENERATED_BODY()

private:
	/// <summary>
	/// Broadcasts the exit buttons event
	/// </summary>
	UFUNCTION()
	void DelegateExitMenu();

	/// <summary>
	/// Opens the audio sub-panel. Connected to audio button event.
	/// </summary>
	UFUNCTION()
	void DelegateOpenAudioPanel();

	/// <summary>
	/// Opens the controls sub-panel. Connected to controls button event
	/// </summary>
	UFUNCTION()
	void DelegateOpenControlsPanel();

	/// <summary>
	/// Opens the main options panel.
	/// </summary>
	UFUNCTION()
	void OpenOptionsPanel();
	
protected:
	////Object References////
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Audio;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Controls;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Exit;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Quit;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class UWidget* ButtonBox;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class UWidget* AudioPanel;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class UWidget* ControlsPanel;

public:
	virtual void NativeConstruct() override;
	virtual void NavigateWindow() override;
};
