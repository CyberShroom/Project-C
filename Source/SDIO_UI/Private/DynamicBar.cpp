// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicBar.h"

void UDynamicBar::UpdateBackgroundBar()
{
	//If the background bar value is greater than the foreground bar value, slowly update it to match. Otherwise, set it equal.
	if (BackgroundBar->GetPercent() - ForegroundBar->GetPercent() > 0.01)
	{
		BackgroundBar->SetPercent(BackgroundBar->GetPercent() - 0.001);
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UDynamicBar::UpdateBackgroundBar);
	}
	else 
	{
		BackgroundBar->SetPercent(ForegroundBar->GetPercent());
	}
}

void UDynamicBar::UpdateText()
{
	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), FMath::RoundToInt(leftVar), FMath::RoundToInt(rightVar))));
	UpdateForegroundBar();
}

void UDynamicBar::UpdateForegroundBar()
{
	ForegroundBar->SetPercent(leftVar / rightVar);
	UpdateBackgroundBar();
}

void UDynamicBar::SetLeftVarValue(float newValue)
{
	leftVar = newValue;
}

void UDynamicBar::SetRightVarValue(float newValue)
{
	rightVar = newValue;
}

void UDynamicBar::InitializeAttributes(FLinearColor backgroundBarColor, FLinearColor foregroundBarColor, float leftVarDefault, float rightVarDefault)
{
	BackgroundBar->SetFillColorAndOpacity(backgroundBarColor);
	ForegroundBar->SetFillColorAndOpacity(foregroundBarColor);
	leftVar = leftVarDefault;
	rightVar = rightVarDefault;

	UpdateText();
}
