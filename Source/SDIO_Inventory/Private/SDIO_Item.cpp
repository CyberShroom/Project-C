// Fill out your copyright notice in the Description page of Project Settings.


#include "SDIO_Item.h"

void USDIO_Item::Initialize()
{
	instanceID.NewGuid();

    if (IsValid(OutlineMaterial))
    {
        //Intially Setup the material
        material = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("The item, %s, does not have a material set in the editor. No material will be applied to this sprite."), *name.ToString());
    }
}
