// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "TTC_Item.generated.h"

/**
 * 
 */
UCLASS()
class TOTHECENTER_API UTTC_Item : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	FText name;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	UTexture2D* sprite;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Internal Information")
	FGuid itemID;

public:
	UFUNCTION(BlueprintPure, Category = "SDIO_Item", meta = (Tooltip = "Returns the Guid of this item."))
	FGuid getID();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
};
