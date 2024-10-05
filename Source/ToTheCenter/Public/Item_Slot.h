// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTC_Enums.h"
#include "Components/Image.h"
#include "TTC_Item.h"
#include "Item_Slot.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API UItem_Slot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SDIO_Items", meta = (Tooltip = "The item contained within this slot."))
	UTTC_Item* containedItem;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (Tooltip = "Defines what kind of items can be stored in this slot."))
	ESlotType type = ESlotType::Universal;

	UFUNCTION(BlueprintCallable, Category = "SDIO_UI", meta = (Tooltip = "Sets the sprite of an image object. Intended to be used with child classes of Item_Slot."))
	void SetSprite(UImage* imageObject, UTexture2D* spriteObject);

	UFUNCTION()
	bool GetContainedItem(UTTC_Item*& existingItem);

	UFUNCTION()
	void SetContainedItem(UTTC_Item* newItem);
};
