// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "DynamicBar.generated.h"

/**
 * 
 */
UCLASS()
class SDIO_UI_API UDynamicBar : public UUserWidget
{
	GENERATED_BODY()
	
	private:
		UPROPERTY()
		float leftVar = 0;

		UPROPERTY()
		float rightVar = 0;

		/// <summary>
		/// Updates the text on the bar.
		/// </summary>
		UFUNCTION()
		void UpdateBackgroundBar();

	protected:
		////Object References////
		UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
		class UProgressBar* BackgroundBar;
		
		UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
		class UProgressBar* ForegroundBar;

		UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
		class UTextBlock* TextBlock;

	public:
		/// <summary>
		/// Updates the text on the bar.
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "SDIO_UI", meta = (Tooltip = "Updates the text on the bar."))
		void UpdateText();

		/// <summary>
		/// Updates the foreground bar.
		/// </summary>
		UFUNCTION(BlueprintCallable, Category = "SDIO_UI", meta = (Tooltip = "Updates the foreground bar."))
		void UpdateForegroundBar();

		UFUNCTION(BlueprintCallable, Category = "SDIO_UI")
		void SetLeftVarValue(float newValue);

		UFUNCTION(BlueprintCallable, Category = "SDIO_UI")
		void SetRightVarValue(float newValue);

		UFUNCTION(BlueprintCallable, Category = "SDIO_UI")
		void InitializeAttributes(FLinearColor backgroundBarColor, FLinearColor foregroundBarColor, float leftVarDefault, float rightVarDefault);

};
