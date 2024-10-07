// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "TTC_Item.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TOTHECENTER_API UTTC_Item : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Internal Information")
	FText name;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Internal Information")
	UTexture2D* sprite = nullptr;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Internal Information")
	FGuid itemID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Internal Information")
	FString instanceID;

public:
	UFUNCTION(BlueprintPure, Category = "SDIO_Item", meta = (Tooltip = "Returns the Guid of this item."))
	FGuid getID();

	UFUNCTION(BlueprintPure, Category = "SDIO_Item", meta = (Tooltip = "Returns the Instance ID of this item. Used to identify the exact item."))
	FString GetInstance();

	UFUNCTION(BlueprintPure, Category = "SDIO_Item", meta = (Tooltip = "Returns the sprite of this item."))
	UTexture2D* GetSprite();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
};
