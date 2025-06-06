// Fill out your copyright notice in the Description page of Project Settings.


#include "TTC_Item.h"

FLinearColor UTTC_Item::GetRarityColor(ERarity item)
{
    switch (item)
    {
        case ERarity::COMMON:
            return FLinearColor::White;
        case ERarity::UNCOMMON:
            return FLinearColor::Green;
        case ERarity::RARE:
            return FLinearColor::Blue;
        case ERarity::EPIC:
            return FColor::Purple.ReinterpretAsLinear();
        case ERarity::LEGENDARY:
            return FColor::Orange.ReinterpretAsLinear();
        case ERarity::FUSION:
            return FColor::Turquoise.ReinterpretAsLinear();
        case ERarity::CORRUPT:
            return FColor::Magenta.ReinterpretAsLinear();
        default:
            break;
    }

    return FLinearColor::Black;
}

void UTTC_Item::Initialize()
{
    Super::Initialize();

    if (IsValid(material))
    {
        material->SetTextureParameterValue("Source", sprite);
        material->SetVectorParameterValue("RarityColor", GetRarityColor(rarity));

        if (sprite->GetSizeX() == sprite->GetSizeY())
        {
            material->SetScalarParameterValue("TexelX", sprite->GetSizeX() / 4);
            material->SetScalarParameterValue("TexelY", sprite->GetSizeY() / 4);
        }
        else
        {
            material->SetScalarParameterValue("TexelX", sprite->GetSizeX());
            material->SetScalarParameterValue("TexelY", sprite->GetSizeY());
        }
    }
}
