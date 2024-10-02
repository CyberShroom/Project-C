// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Window.h"
#include "SDIO_UI_TextButton.h"
#include "SDIO_UIWindow_Title.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API USDIO_UIWindow_Title : public UUI_Window
{
	GENERATED_BODY()

private:
	//UFUNCTION()
	//void StartSingleplayer();
	UFUNCTION()
	void OpenMultiplayer();
	UFUNCTION()
	void OpenOptions();
	//UFUNCTION()
	//void ExitGame();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Singleplayer;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Multiplayer;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Options;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class USDIO_UI_TextButton* Quit;

public:
	virtual void NativeConstruct() override;
};
