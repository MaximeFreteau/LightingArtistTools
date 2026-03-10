// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Light.h"
#include "Components/LightComponent.h"
#include "ColorPalette.h"
#include "LightingScenario.h"
#include "LightingToolsLibraries.generated.h"

UCLASS()
class LIGHTINGARTISTTOOLS_API ULightingToolsLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Lighting Tools")
    static TArray<AActor*> GetAllLightsInLevel();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools")
    static void SetLightsIntensity(const TArray<AActor*>& Lights, float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools")
    static void SetLightsColor(const TArray<AActor*>& Lights, FLinearColor Color);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools")
    static void SetLightsEnabled(const TArray<AActor*>& Lights, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Light Properties")
    static void SetLightAttenuationRadius(AActor* LightActor, float Radius);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lighting Tools|Light Properties")
    static float GetLightAttenuationRadius(AActor* LightActor);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools")
    static void GetLightsByType(
        TArray<AActor*>& PointLights,
        TArray<AActor*>& SpotLights,
        TArray<AActor*>& DirectionalLights,
        TArray<AActor*>& SkyLights,
        TArray<AActor*>& RectLights
    );

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Selection")
    static void GetSelectedLightsInfo(
        int32& OutTotalSelected,
        int32& OutPointLights,
        int32& OutSpotLights,
        int32& OutDirectionalLights,
        int32& OutRectLights,
        int32& OutSkyLights
    );

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools")
    static ULightingScenario* SaveCurrentLightingToScenario(const FString& ScenarioName, const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools")
    static void LoadLightingScenario(ULightingScenario* Scenario);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools")
    static ULightingScenario* CreateLightingScenarioAsset(const FString& AssetName, const FString& PackagePath);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools", meta = (CallInEditor = "true"))
    static void BuildLighting();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Levels")
    static FString GetCurrentMapName();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Levels")
    static void LoadSubLevel(TSoftObjectPtr<UWorld> SubLevel);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Levels")
    static void UnloadSubLevel(TSoftObjectPtr<UWorld> SubLevel);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Levels")
    static void ActivateDataLayer(FName DataLayerName);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Levels")
    static void DeactivateDataLayer(FName DataLayerName);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Scenarios")
    static TArray<ULightingScenario*> GetScenariosForCurrentMap();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Scenarios")
    static void RecreateMissingLights(ULightingScenario* Scenario);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Scenarios")
    static AActor* RecreateLightFromSettings(const FLightSettings& LightSettings);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Scenarios")
    static int32 CountMissingLights(ULightingScenario* Scenario);
    
    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Scenarios")
    static ULightingScenario* CaptureCompleteScenario(const FString& ScenarioName, const FString& Description);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Scenarios")
    static void ApplyCompleteScenario(ULightingScenario* Scenario);
    
    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Scenarios")
    static bool UpdateExistingScenario(ULightingScenario* Scenario);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Fog")
    static AActor* GetExponentialHeightFog();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Fog")
    static FFogSettings_Saved CaptureFogSettings();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Fog")
    static void ApplyFogSettings(const FFogSettings_Saved& FogSettings);
    
    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Sky")
    static AActor* GetSkyLight();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Sky")
    static FSkyLightSettings_Saved CaptureSkyLightSettings();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Sky")
    static void ApplySkyLightSettings(const FSkyLightSettings_Saved& SkySettings);
    
    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Sun")
    static AActor* GetDirectionalLight();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Sun")
    static FDirectionalLightSettings_Saved CaptureDirectionalLightSettings();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Sun")
    static void ApplyDirectionalLightSettings(const FDirectionalLightSettings_Saved& SunSettings);
    
    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|PostProcess")
    static TArray<APostProcessVolume*> GetAllPostProcessVolumes();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|PostProcess")
    static FPostProcessSettings_Saved CapturePostProcessSettings(APostProcessVolume* PPVolume);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|PostProcess")
    static void ApplyPostProcessSettings(APostProcessVolume* PPVolume, const FPostProcessSettings_Saved& Settings);
    
    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Utility")
    static void SelectActorInWorld(AActor* Actor);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lighting Tools|Utility")
    static bool IsActorSelected(AActor* Actor);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Utility")
    static TArray<ULightingScenario*> GetAllScenarioAssets();
    
    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Color Palettes")
    static TArray<UColorPaletteAsset*> GetAllColorPalettes();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Color Palettes")
    static UColorPaletteAsset* GetColorPaletteByName(const FString& PaletteName);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Color Palettes")
    static void ApplyColorToSelectedLights(FLinearColor Color, float Temperature = 6500.0f);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Color Palettes")
    static UColorPaletteAsset* CreateColorPaletteAsset(const FString& PaletteName, const FString& Category);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static AActor* SpawnLightAtViewport(TSubclassOf<AActor> LightClass, bool bUseViewportPosition, bool bUseViewportRotation);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static void GetViewportCameraTransform(FVector& OutLocation, FRotator& OutRotation);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static AActor* SpawnPointLight(FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static AActor* SpawnSpotLight(FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static AActor* SpawnDirectionalLight(FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static AActor* SpawnRectLight(FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static AActor* SpawnExponentialHeightFog(FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static AActor* SpawnSkyLight(FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Spawn")
    static AActor* SpawnPostProcessVolume(FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Quick Access")
    static void SnapSelectedToViewport(bool bSnapPosition, bool bSnapRotation);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Quick Access")
    static void AlignSelectedToSurface(float TraceDistance = 10000.0f);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Quick Access")
    static TArray<AActor*> GetSelectedActors();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Performance")
    static void SetViewMode(const FString& ViewModeName);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Performance")
    static void OpenGPUProfiler();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Performance")
    static void ToggleStatUnit();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Performance")
    static void ToggleStatFPS();

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Performance")
    static void GetLightStatistics(
        int32& OutTotalLights,
        int32& OutShadowCastingLights,
        int32& OutStaticLights,
        int32& OutStationaryLights,
        int32& OutMovableLights
    );

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Performance")
    static void ConvertLightsToPhysicalUnits(const TArray<AActor*>& Lights);

    UFUNCTION(BlueprintCallable, Category = "Lighting Tools|Performance")
    static void OptimizeShadowSettings(const TArray<AActor*>& Lights);
};