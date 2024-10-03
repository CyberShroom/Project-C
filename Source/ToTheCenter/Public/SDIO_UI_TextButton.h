// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SDIO_UI_TextButton.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonClicked);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateText);

UCLASS()
class TOTHECENTER_API USDIO_UI_TextButton : public UUserWidget
{
	GENERATED_BODY()

private:
	////Event Delegates////
	UFUNCTION()
	void OnButtonClick();
	UFUNCTION()
	void OnButtonHover();
	UFUNCTION()
	void OnButtonUnhover();

	/// <summary>Sets the text of the text object to ButtonText</summary>
	UFUNCTION()
	void SetNewText();

protected:
	////Object References////
	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class UButton* TextButton;

	UPROPERTY(BlueprintReadOnly, Category = "Hierarchy References", meta = (BindWidget))
	class UTextBlock* TextButtonText;

public:
	/// <summary>The text of the button. Must run SetNewText() in order to update it in-game.</summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "The text to display."))
	FText ButtonText = FText::FromString("Text");

	/// <summary>Event Signature. Called whenever this object gets clicked.</summary>
	UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Event Dispatchers", meta = (Tooltip = "An event that is called when a text button is clicked."))
	FButtonClicked OnButtonClicked;

	//UPROPERTY(BlueprintAssignable, Category = "SDIO_UI | Event Dispatchers", meta = (Tooltip = "An event that updates the Text Buttons Text when called."))
	//FUpdateText OnUpdateText;

	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
};
