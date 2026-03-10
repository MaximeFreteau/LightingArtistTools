// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorPalette.h"
#include "DefaultColorPalettes.generated.h"

UCLASS()
class LIGHTINGARTISTTOOLS_API UDefaultColorPalettes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Setup")
	static void CreateDefaultPalettes();

	static UColorPaletteAsset* CreateCinematicPalette();
	static UColorPaletteAsset* CreateNaturalLightingPalette();
	static UColorPaletteAsset* CreateStudioLightingPalette();
	static UColorPaletteAsset* CreateFantasyPalette();
	static UColorPaletteAsset* CreateHorrorPalette();
	static UColorPaletteAsset* CreateSciFiPalette();
};