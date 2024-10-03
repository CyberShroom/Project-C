// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Window.h"
#include "SDIO_UI_TextButton.h"
#include "SDIO_UIWindow_Options.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API USDIO_UIWindow_Options : public UUI_Window
{
	GENERATED_BODY()

private:
	////Event Delegates For Text_Buttons////
	UFUNCTION()
	void ExitMenu();
	UFUNCTION()
	void OpenAudioPanel();
	UFUNCTION()
	void OpenControlsPanel();
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

	/// <summary>Reference to the UI_Managers defaultUI</summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	EUIID defaultUI = EUIID::NoID;
};
