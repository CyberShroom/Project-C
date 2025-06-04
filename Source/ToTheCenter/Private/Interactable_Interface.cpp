// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable_Interface.h"

// Add default functionality here for any IInteractable_Interface functions that are not pure virtual.

void IInteractable_Interface::CheckProx(FVector actorLocation, UWidgetComponent* widget, UWorld* worldRef)
{
	if (FVector::Dist(actorLocation, UGameplayStatics::GetPlayerController(worldRef, 0)->GetPawn()->GetActorLocation()) <= 500)
	{
		widget->SetTintColorAndOpacity(FLinearColor(1, 1, 1, 1));
	}
	else
	{
		widget->SetTintColorAndOpacity(FLinearColor(1, 1, 1, 0));
	}
}
