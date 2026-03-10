// Copyright Maxime Freteau. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Scene.h"
#include "LightingScenario.generated.h"

class AActor;
class APostProcessVolume;

USTRUCT(BlueprintType)
struct FLightSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    FString LightName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    FString LightType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    TSoftObjectPtr<AActor> LightActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
    FVector Scale = FVector::OneVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float Intensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    uint8 IntensityUnits = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    FLinearColor LightColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float Temperature = 6500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bUseTemperature = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float VolumetricScatteringIntensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bAffectsWorld = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bIsEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float AttenuationRadius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bUseInverseSquaredFalloff = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float LightFalloffExponent = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastShadows = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastStaticShadows = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastDynamicShadows = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastVolumetricShadow = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastDeepShadow = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastModulatedShadows = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastShadowsFromCinematicObjectsOnly = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float ShadowResolutionScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float ShadowBias = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float ShadowSlopeBias = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float ShadowSharpen = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float ContactShadowLength = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bContactShadowLengthInWS = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float SourceRadius = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float SoftSourceRadius = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float SourceLength = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float InnerConeAngle = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float OuterConeAngle = 44.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float SourceWidth = 64.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float SourceHeight = 64.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float BarnDoorAngle = 88.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float BarnDoorLength = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bUseLightFunction = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    FVector LightFunctionScale = FVector::OneVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float LightFunctionFadeDistance = 100000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float DisabledBrightness = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bUseIESProfile = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float IESBrightnessScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float IndirectLightingIntensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float ShadowExponent = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bVisible = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bVisibleInReflectionCaptures = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bVisibleInRayTracing = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bVisibleInRealTimeSkyCaptures = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bAffectReflection = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bAffectGlobalIllumination = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float DeepShadowLayerDistribution = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    int32 SamplesPerPixel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    int32 DynamicShadowDistanceMovableLight = 20000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    int32 DynamicShadowDistanceStationaryLight = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    int32 DynamicShadowCascades = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CascadeDistributionExponent = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CascadeTransitionFraction = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float ShadowDistanceFadeoutFraction = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bUseInsetShadowsForMovableObjects = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    int32 FarShadowCascadeCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float FarShadowDistance = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bAtmosphereSunLight = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    int32 AtmosphereSunLightIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    FLinearColor AtmosphereSunDiskColorScale = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bPerPixelAtmosphereTransmittance = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastShadowsOnClouds = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastShadowsOnAtmosphere = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bCastCloudShadows = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CloudShadowStrength = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CloudShadowOnAtmosphereStrength = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CloudShadowOnSurfaceStrength = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CloudShadowDepthBias = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CloudShadowExtent = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CloudShadowMapResolutionScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float CloudShadowRaySampleCountScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bEnableLightShaftOcclusion = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float OcclusionMaskDarkness = 0.05f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float OcclusionDepthRange = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    FVector LightShaftOverrideDirection = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float RayTracingMaxBias = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    int32 SamplesPerPixelRT = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    uint8 Mobility = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    float SpecularScale = 1.0f;
};

USTRUCT(BlueprintType)
struct FFogSettings_Saved
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    float FogDensity = 0.02f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    FLinearColor FogInscatteringColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    float FogHeightFalloff = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    float FogMaxOpacity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    float StartDistance = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    float DirectionalInscatteringExponent = 4.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fog")
    FLinearColor DirectionalInscatteringColor = FLinearColor::White;
};

USTRUCT(BlueprintType)
struct FSkyLightSettings_Saved
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky Light")
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky Light")
    float Intensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky Light")
    FLinearColor Color = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky Light")
    float IndirectLightingIntensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky Light")
    float VolumetricScatteringIntensity = 1.0f;
};

USTRUCT(BlueprintType)
struct FDirectionalLightSettings_Saved
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    float Intensity = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    FLinearColor Color = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    float Temperature = 6500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    bool bUseTemperature = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    bool bCastShadows = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    float IndirectLightingIntensity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun")
    float VolumetricScatteringIntensity = 1.0f;
};

USTRUCT(BlueprintType)
struct FPostProcessSettings_Saved
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    FString PostProcessName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    TSoftObjectPtr<APostProcessVolume> PostProcessVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process|Bloom")
    float BloomIntensity = 0.675f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process|Exposure")
    float Exposure = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process|Color")
    float Saturation = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process|Color")
    float Contrast = 1.0f;
};

UENUM(BlueprintType)
enum class EScenarioLevelType : uint8
{
    None            UMETA(DisplayName = "None"),
    SubLevel        UMETA(DisplayName = "Sub Level"),
    DataLayer       UMETA(DisplayName = "Data Layer")
};

USTRUCT(BlueprintType)
struct FScenarioLevelInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    EScenarioLevelType LevelType = EScenarioLevelType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (EditCondition = "LevelType == EScenarioLevelType::SubLevel", EditConditionHides))
    TSoftObjectPtr<UWorld> SubLevelToLoad;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level", meta = (EditCondition = "LevelType == EScenarioLevelType::DataLayer", EditConditionHides))
    FName DataLayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
    bool bAutoLoadLevel = true;
};

UCLASS(BlueprintType)
class LIGHTINGARTISTTOOLS_API ULightingScenario : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scenario Info")
    FString ScenarioName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scenario Info", meta = (MultiLine = true))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scenario Info")
    FDateTime CreationDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scenario Info")
    FString TargetMapName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scenario Info")
    FScenarioLevelInfo LevelInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
    TArray<FLightSettings> AllLights;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FFogSettings_Saved FogSettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FSkyLightSettings_Saved SkyLightSettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
    FDirectionalLightSettings_Saved SunSettings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Post Process")
    TArray<FPostProcessSettings_Saved> PostProcessVolumes;
};