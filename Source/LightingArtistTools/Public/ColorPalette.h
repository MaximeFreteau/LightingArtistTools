// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ColorPalette.generated.h"

USTRUCT(BlueprintType)
struct FColorPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FString ColorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	float Temperature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
	FString Description;

	FColorPreset()
		: ColorName("White")
		, Color(FLinearColor::White)
		, Temperature(6500.0f)
		, Description("")
	{
	}
};

USTRUCT(BlueprintType)
struct FColorPaletteRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
	FString PaletteName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
	TArray<FColorPreset> Colors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
	FString Category;

	FColorPaletteRow()
		: PaletteName("Default Palette")
		, Category("General")
	{
	}
};

UCLASS(BlueprintType)
class LIGHTINGARTISTTOOLS_API UColorPaletteAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
	FString PaletteName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
	FString Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
	TArray<FColorPreset> Colors;
};