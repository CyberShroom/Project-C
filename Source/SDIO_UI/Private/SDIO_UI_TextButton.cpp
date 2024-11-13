// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_UI_TextButton.h"

void USDIO_UI_TextButton::NativeConstruct()
{
	Super::NativeConstruct();

	//Checks if object is loaded
	if (TextButton)
	{
		TextButton->OnClicked.AddUniqueDynamic(this, &USDIO_UI_TextButton::OnButtonClick);
		TextButton->OnHovered.AddUniqueDynamic(this, &USDIO_UI_TextButton::OnButtonHover);
		TextButton->OnUnhovered.AddUniqueDynamic(this, &USDIO_UI_TextButton::OnButtonUnhover);
		//OnUpdateText.AddUniqueDynamic(this, &USDIO_UI_TextButton::SetNewText);
		SetNewText();
	}
}

void USDIO_UI_TextButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	//Checks if object is loaded
	if (TextButton)
	{
		SetNewText();
	}
}

void USDIO_UI_TextButton::OnButtonClick()
{
	OnButtonClicked.Broadcast();
}

void USDIO_UI_TextButton::OnButtonHover()
{
	TextButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
}

void USDIO_UI_TextButton::OnButtonUnhover()
{
	TextButtonText->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
}

void USDIO_UI_TextButton::SetNewText()
{
	TextButtonText->SetText(ButtonText);
}
