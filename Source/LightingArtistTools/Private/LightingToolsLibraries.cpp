// Copyright Maxime Freteau. All Rights Reserved.

#include "LightingToolsLibraries.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Editor.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/PostProcessVolume.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"
#include "LightingBuildOptions.h"
#include "LightingScenario.h"
#include "ColorPalette.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Misc/Paths.h"
#include "Misc/PackageName.h"
#include "Editor/EditorEngine.h"
#include "Engine/PointLight.h"
#include "Engine/SpotLight.h"
#include "Engine/DirectionalLight.h"
#include "Engine/RectLight.h"
#include "Engine/SkyLight.h"
#include "LevelEditorViewport.h"
#include "EditorViewportClient.h"
#include "DrawDebugHelpers.h"
#include "Selection.h"
#include "Engine/LevelStreaming.h"
#include "WorldPartition/DataLayer/DataLayerSubsystem.h"
#include "WorldPartition/DataLayer/DataLayerAsset.h"
#include "WorldPartition/DataLayer/DataLayerInstance.h"
#include "UnrealClient.h"
#include "Editor.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "LevelEditor.h"
#include "Modules/ModuleManager.h"
#include "Selection.h"
#include "UObject/SavePackage.h"
#include "EditorLevelUtils.h"
#include "LevelEditorViewport.h"
#include "EditorViewportClient.h"
#include "Kismet2/DebuggerCommands.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ScopedTransaction.h"


TArray<AActor*> ULightingToolsLibrary::GetAllLightsInLevel()
{
    TArray<AActor*> LightActors;
    
    if (GEditor)
    {
        UWorld* World = GEditor->GetEditorWorldContext().World();
        if (World)
        {
            for (TActorIterator<AActor> It(World); It; ++It)
            {
                AActor* Actor = *It;
                if (Actor && Actor->FindComponentByClass<ULightComponent>())
                {
                    LightActors.Add(Actor);
                }
            }
        }
    }
    
    return LightActors;
}

void ULightingToolsLibrary::GetSelectedLightsInfo(
    int32& OutTotalSelected,
    int32& OutPointLights,
    int32& OutSpotLights,
    int32& OutDirectionalLights,
    int32& OutRectLights,
    int32& OutSkyLights)
{
    OutTotalSelected = 0;
    OutPointLights = 0;
    OutSpotLights = 0;
    OutDirectionalLights = 0;
    OutRectLights = 0;
    OutSkyLights = 0;

    if (!GEditor)
    {
        return;
    }

    TArray<AActor*> SelectedActors;
    GEditor->GetSelectedActors()->GetSelectedObjects(SelectedActors);
    
    for (AActor* Actor : SelectedActors)
    {
        if (!Actor) continue;

        ULightComponent* LightComp = Actor->FindComponentByClass<ULightComponent>();
        if (!LightComp) continue;

        OutTotalSelected++;

        if (Actor->FindComponentByClass<UPointLightComponent>())
        {
            OutPointLights++;
        }
        else if (Actor->FindComponentByClass<USpotLightComponent>())
        {
            OutSpotLights++;
        }
        else if (Actor->FindComponentByClass<UDirectionalLightComponent>())
        {
            OutDirectionalLights++;
        }
        else if (Actor->FindComponentByClass<URectLightComponent>())
        {
            OutRectLights++;
        }
        else if (Actor->FindComponentByClass<USkyLightComponent>())
        {
            OutSkyLights++;
        }
    }
}

void ULightingToolsLibrary::SetLightsIntensity(const TArray<AActor*>& Lights, float Intensity)
{
    if (Lights.Num() == 0) return;

    FScopedTransaction Transaction(FText::FromString("Set Lights Intensity"));

    for (AActor* LightActor : Lights)
    {
        if (LightActor)
        {
            ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
            if (LightComp)
            {
                LightComp->Modify();
                LightComp->SetIntensity(Intensity);
                LightComp->MarkRenderStateDirty();
            }
        }
    }
}

void ULightingToolsLibrary::SetLightsColor(const TArray<AActor*>& Lights, FLinearColor Color)
{
    if (Lights.Num() == 0) return;

    FScopedTransaction Transaction(FText::FromString("Set Lights Color"));

    for (AActor* LightActor : Lights)
    {
        if (LightActor)
        {
            ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
            if (LightComp)
            {
                LightComp->Modify();
                LightComp->SetLightColor(Color);
                LightComp->MarkRenderStateDirty();
            }
        }
    }
}

void ULightingToolsLibrary::SetLightsEnabled(const TArray<AActor*>& Lights, bool bEnabled)
{
    if (Lights.Num() == 0) return;

    FScopedTransaction Transaction(FText::FromString(bEnabled ? "Enable Lights" : "Disable Lights"));

    for (AActor* LightActor : Lights)
    {
        if (LightActor)
        {
            ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
            if (LightComp)
            {
                LightComp->Modify();
                LightComp->SetVisibility(bEnabled);
                LightComp->MarkRenderStateDirty();
            }
        }
    }
}

void ULightingToolsLibrary::GetLightsByType(
    TArray<AActor*>& PointLights,
    TArray<AActor*>& SpotLights,
    TArray<AActor*>& DirectionalLights,
    TArray<AActor*>& SkyLights,
    TArray<AActor*>& RectLights)
{
    PointLights.Empty();
    SpotLights.Empty();
    DirectionalLights.Empty();
    SkyLights.Empty();
    RectLights.Empty();
    
    TArray<AActor*> AllLights = GetAllLightsInLevel();
    
    for (AActor* LightActor : AllLights)
    {
        if (!LightActor) continue;
        
        if (LightActor->FindComponentByClass<USpotLightComponent>())
            SpotLights.Add(LightActor);
        else if (LightActor->FindComponentByClass<URectLightComponent>())
            RectLights.Add(LightActor);
        else if (LightActor->FindComponentByClass<UPointLightComponent>())
            PointLights.Add(LightActor);
        else if (LightActor->FindComponentByClass<UDirectionalLightComponent>())
            DirectionalLights.Add(LightActor);
        else if (LightActor->FindComponentByClass<USkyLightComponent>())
            SkyLights.Add(LightActor);
    }
}

void ULightingToolsLibrary::BuildLighting()
{
    if (GUnrealEd)
    {
        FLightingBuildOptions LightingOptions;
        
        LightingOptions.QualityLevel = ELightingBuildQuality::Quality_Production;
        LightingOptions.bShowLightingBuildInfo = true;
        LightingOptions.bOnlyBuildSelected = false;
        LightingOptions.bOnlyBuildCurrentLevel = false;
        LightingOptions.bOnlyBuildSelectedLevels = false;
        LightingOptions.bOnlyBuildVisibility = false;
        LightingOptions.bUseErrorColoring = true;
        
        GUnrealEd->BuildLighting(LightingOptions);
    }
}

ULightingScenario* ULightingToolsLibrary::SaveCurrentLightingToScenario(const FString& ScenarioName, const FString& Description)
{
    FString PackagePath = FString::Printf(TEXT("/Game/LightingTools/LightingScenarios/%s"), *ScenarioName);
    ULightingScenario* NewScenario = CreateLightingScenarioAsset(ScenarioName, PackagePath);
    
    if (!NewScenario)
    {
        return nullptr;
    }

    NewScenario->ScenarioName = ScenarioName;
    NewScenario->Description = Description;
    NewScenario->CreationDate = FDateTime::Now();
    NewScenario->AllLights.Empty();

    TArray<AActor*> AllLights = GetAllLightsInLevel();
    for (AActor* LightActor : AllLights)
    {
        if (!LightActor) continue;

        ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
        if (!LightComp) continue;

        FLightSettings LightSettings;
        LightSettings.LightName = LightActor->GetName();
        LightSettings.LightActor = LightActor;
        LightSettings.Intensity = LightComp->Intensity;
        LightSettings.LightColor = LightComp->GetLightColor();
        LightSettings.bIsEnabled = LightComp->IsVisible();

        if (LightActor->FindComponentByClass<USpotLightComponent>())
            LightSettings.LightType = "SpotLight";
        else if (LightActor->FindComponentByClass<URectLightComponent>())
            LightSettings.LightType = "RectLight";
        else if (LightActor->FindComponentByClass<UPointLightComponent>())
            LightSettings.LightType = "PointLight";
        else if (LightActor->FindComponentByClass<UDirectionalLightComponent>())
            LightSettings.LightType = "DirectionalLight";
        else if (LightActor->FindComponentByClass<USkyLightComponent>())
            LightSettings.LightType = "SkyLight";

        NewScenario->AllLights.Add(LightSettings);
    }

    NewScenario->MarkPackageDirty();
        
    return NewScenario;
}

void ULightingToolsLibrary::LoadLightingScenario(ULightingScenario* Scenario)
{
    if (!Scenario)
    {
        return;
    }
    
    FScopedTransaction Transaction(FText::FromString("Load Lighting Scenario"));

    int32 LoadedCount = 0;
    int32 MissingCount = 0;

    for (const FLightSettings& LightSettings : Scenario->AllLights)
    {
        AActor* LightActor = LightSettings.LightActor.Get();
        
        if (!LightActor)
        {
            MissingCount++;
            continue;
        }

        ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
        if (!LightComp)
        {
            continue;
        }

        LightComp->Modify();
        LightActor->Modify();

        LightActor->SetActorLocation(LightSettings.Location);
        LightActor->SetActorRotation(LightSettings.Rotation);
        LightActor->SetActorScale3D(LightSettings.Scale);

        LightComp->SetIntensity(LightSettings.Intensity);
        LightComp->SetLightColor(LightSettings.LightColor);
        LightComp->SetTemperature(LightSettings.Temperature);
        LightComp->bUseTemperature = LightSettings.bUseTemperature;
        LightComp->VolumetricScatteringIntensity = LightSettings.VolumetricScatteringIntensity;
        LightComp->bAffectsWorld = LightSettings.bAffectsWorld;
        LightComp->SetVisibility(LightSettings.bIsEnabled);

        LightComp->SetCastShadows(LightSettings.bCastShadows);
        LightComp->CastStaticShadows = LightSettings.bCastStaticShadows;
        LightComp->CastDynamicShadows = LightSettings.bCastDynamicShadows;
        LightComp->bCastVolumetricShadow = LightSettings.bCastVolumetricShadow;
        LightComp->bCastDeepShadow = LightSettings.bCastDeepShadow;
        LightComp->bCastShadowsFromCinematicObjectsOnly = LightSettings.bCastShadowsFromCinematicObjectsOnly;
        LightComp->ShadowResolutionScale = LightSettings.ShadowResolutionScale;
        LightComp->ShadowBias = LightSettings.ShadowBias;
        LightComp->ShadowSlopeBias = LightSettings.ShadowSlopeBias;
        LightComp->ShadowSharpen = LightSettings.ShadowSharpen;
        LightComp->ContactShadowLength = LightSettings.ContactShadowLength;
        LightComp->ContactShadowLengthInWS = LightSettings.bContactShadowLengthInWS;

        LightComp->LightFunctionScale = LightSettings.LightFunctionScale;
        LightComp->LightFunctionFadeDistance = LightSettings.LightFunctionFadeDistance;
        LightComp->DisabledBrightness = LightSettings.DisabledBrightness;

        LightComp->bUseIESBrightness = LightSettings.bUseIESProfile;
        LightComp->IESBrightnessScale = LightSettings.IESBrightnessScale;

        LightComp->IndirectLightingIntensity = LightSettings.IndirectLightingIntensity;
        LightComp->SetVisibility(LightSettings.bVisible);
        LightComp->bAffectReflection = LightSettings.bAffectReflection;
        LightComp->bAffectGlobalIllumination = LightSettings.bAffectGlobalIllumination;
        LightComp->DeepShadowLayerDistribution = LightSettings.DeepShadowLayerDistribution;
        LightComp->SamplesPerPixel = LightSettings.SamplesPerPixel;

        LightComp->SetMobility((EComponentMobility::Type)LightSettings.Mobility);

        LightComp->SpecularScale = LightSettings.SpecularScale;
        
        if (LightSettings.LightType == "PointLight")
        {
            if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(LightComp))
            {
                PointLight->IntensityUnits = (ELightUnits)LightSettings.IntensityUnits;
                PointLight->AttenuationRadius = LightSettings.AttenuationRadius;
                PointLight->bUseInverseSquaredFalloff = LightSettings.bUseInverseSquaredFalloff;
                PointLight->LightFalloffExponent = LightSettings.LightFalloffExponent;
                PointLight->SourceRadius = LightSettings.SourceRadius;
                PointLight->SoftSourceRadius = LightSettings.SoftSourceRadius;
                PointLight->SourceLength = LightSettings.SourceLength;
                
#if WITH_EDITOR
                FPropertyChangedEvent PropChangeEvent(nullptr);
                PointLight->PostEditChangeProperty(PropChangeEvent);
#endif

            }
        }
        else if (LightSettings.LightType == "SpotLight")
        {
            if (USpotLightComponent* SpotLight = Cast<USpotLightComponent>(LightComp))
            {
                SpotLight->IntensityUnits = (ELightUnits)LightSettings.IntensityUnits;
                SpotLight->AttenuationRadius = LightSettings.AttenuationRadius;
                SpotLight->bUseInverseSquaredFalloff = LightSettings.bUseInverseSquaredFalloff;
                SpotLight->LightFalloffExponent = LightSettings.LightFalloffExponent;
                SpotLight->InnerConeAngle = LightSettings.InnerConeAngle;
                SpotLight->OuterConeAngle = LightSettings.OuterConeAngle;
                SpotLight->SourceRadius = LightSettings.SourceRadius;
                SpotLight->SoftSourceRadius = LightSettings.SoftSourceRadius;
                SpotLight->SourceLength = LightSettings.SourceLength;
                
#if WITH_EDITOR
                FPropertyChangedEvent PropChangeEvent(nullptr);
                SpotLight->PostEditChangeProperty(PropChangeEvent);
#endif

            }
        }
        else if (LightSettings.LightType == "RectLight")
        {
            if (URectLightComponent* RectLight = Cast<URectLightComponent>(LightComp))
            {
                RectLight->IntensityUnits = (ELightUnits)LightSettings.IntensityUnits;
                RectLight->AttenuationRadius = LightSettings.AttenuationRadius;
                RectLight->SourceWidth = LightSettings.SourceWidth;
                RectLight->SourceHeight = LightSettings.SourceHeight;
                RectLight->BarnDoorAngle = LightSettings.BarnDoorAngle;
                RectLight->BarnDoorLength = LightSettings.BarnDoorLength;
                
#if WITH_EDITOR
                FPropertyChangedEvent PropChangeEvent(nullptr);
                RectLight->PostEditChangeProperty(PropChangeEvent);
#endif

            }
        }
        else if (LightSettings.LightType == "DirectionalLight")
        {
            if (UDirectionalLightComponent* DirLight = Cast<UDirectionalLightComponent>(LightComp))
            {
                DirLight->DynamicShadowDistanceMovableLight = LightSettings.DynamicShadowDistanceMovableLight;
                DirLight->DynamicShadowDistanceStationaryLight = LightSettings.DynamicShadowDistanceStationaryLight;
                DirLight->DynamicShadowCascades = LightSettings.DynamicShadowCascades;
                DirLight->CascadeDistributionExponent = LightSettings.CascadeDistributionExponent;
                DirLight->CascadeTransitionFraction = LightSettings.CascadeTransitionFraction;
                DirLight->ShadowDistanceFadeoutFraction = LightSettings.ShadowDistanceFadeoutFraction;
                DirLight->bUseInsetShadowsForMovableObjects = LightSettings.bUseInsetShadowsForMovableObjects;
                DirLight->FarShadowCascadeCount = LightSettings.FarShadowCascadeCount;
                DirLight->FarShadowDistance = LightSettings.FarShadowDistance;

                DirLight->bAtmosphereSunLight = LightSettings.bAtmosphereSunLight;
                DirLight->AtmosphereSunLightIndex = LightSettings.AtmosphereSunLightIndex;
                DirLight->AtmosphereSunDiskColorScale = LightSettings.AtmosphereSunDiskColorScale;
                DirLight->bPerPixelAtmosphereTransmittance = LightSettings.bPerPixelAtmosphereTransmittance;
                DirLight->bCastShadowsOnClouds = LightSettings.bCastShadowsOnClouds;
                DirLight->bCastShadowsOnAtmosphere = LightSettings.bCastShadowsOnAtmosphere;
                DirLight->bCastCloudShadows = LightSettings.bCastCloudShadows;
                DirLight->CloudShadowStrength = LightSettings.CloudShadowStrength;
                DirLight->CloudShadowOnAtmosphereStrength = LightSettings.CloudShadowOnAtmosphereStrength;
                DirLight->CloudShadowOnSurfaceStrength = LightSettings.CloudShadowOnSurfaceStrength;
                DirLight->CloudShadowDepthBias = LightSettings.CloudShadowDepthBias;
                DirLight->CloudShadowExtent = LightSettings.CloudShadowExtent;
                DirLight->CloudShadowMapResolutionScale = LightSettings.CloudShadowMapResolutionScale;
                DirLight->CloudShadowRaySampleCountScale = LightSettings.CloudShadowRaySampleCountScale;

                DirLight->bEnableLightShaftOcclusion = LightSettings.bEnableLightShaftOcclusion;
                DirLight->OcclusionMaskDarkness = LightSettings.OcclusionMaskDarkness;
                DirLight->OcclusionDepthRange = LightSettings.OcclusionDepthRange;
                DirLight->LightShaftOverrideDirection = LightSettings.LightShaftOverrideDirection;
                
#if WITH_EDITOR
                FPropertyChangedEvent PropChangeEvent(nullptr);
                DirLight->PostEditChangeProperty(PropChangeEvent);
#endif
            }
        }
        else if (LightSettings.LightType == "SkyLight")
        {
            if (USkyLightComponent* SkyLight = Cast<USkyLightComponent>(LightComp))
            {
#if WITH_EDITOR
                FPropertyChangedEvent PropChangeEvent(nullptr);
                SkyLight->PostEditChangeProperty(PropChangeEvent);
#endif
            }
        }

        LightComp->MarkRenderStateDirty();
        LoadedCount++;
    }

    ApplyFogSettings(Scenario->FogSettings);
    ApplySkyLightSettings(Scenario->SkyLightSettings);
    ApplyDirectionalLightSettings(Scenario->SunSettings);

    TArray<APostProcessVolume*> PPVolumes = GetAllPostProcessVolumes();
    for (int32 i = 0; i < FMath::Min(PPVolumes.Num(), Scenario->PostProcessVolumes.Num()); i++)
    {
        ApplyPostProcessSettings(PPVolumes[i], Scenario->PostProcessVolumes[i]);
    }

    if (GEditor)
    {
        GEditor->RedrawAllViewports();
    }
}

ULightingScenario* ULightingToolsLibrary::CreateLightingScenarioAsset(const FString& AssetName, const FString& PackagePath)
{
#if WITH_EDITOR
    FString PackageName = PackagePath;
    if (!PackageName.EndsWith(TEXT("/")))
    {
        PackageName += TEXT("/");
    }
    PackageName += AssetName;

    UPackage* ExistingPackage = FindPackage(nullptr, *PackageName);
    if (ExistingPackage)
    {
        return nullptr;
    }

    UPackage* Package = CreatePackage(*PackageName);
    if (!Package)
    {
        return nullptr;
    }

    ULightingScenario* NewScenario = NewObject<ULightingScenario>(
        Package,
        ULightingScenario::StaticClass(),
        FName(*AssetName),
        RF_Public | RF_Standalone | RF_Transactional
    );

    if (!NewScenario)
    {
        return nullptr;
    }

    Package->MarkPackageDirty();
    
    FAssetRegistryModule::AssetCreated(NewScenario);

    FString PackageFileName = FPackageName::LongPackageNameToFilename(
        PackageName,
        FPackageName::GetAssetPackageExtension()
    );

    FSavePackageArgs SaveArgs;
    SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
    SaveArgs.SaveFlags = SAVE_NoError;

    bool bSaved = UPackage::SavePackage(Package, NewScenario, *PackageFileName, SaveArgs);
    
    if (bSaved)
    {
    }
    else
    {
    }

    return NewScenario;
#else
    return nullptr;
#endif
}

ULightingScenario* ULightingToolsLibrary::CaptureCompleteScenario(const FString& ScenarioName, const FString& Description)
{
    if (!GEditor)
    {
        return nullptr;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World)
    {
        return nullptr;
    }
    
    FString PackagePath = FString::Printf(TEXT("/Game/LightingTools/LightingScenarios/%s"), *ScenarioName);
    ULightingScenario* NewScenario = CreateLightingScenarioAsset(ScenarioName, PackagePath);

    if (!NewScenario)
    {
        return nullptr;
    }

    NewScenario->ScenarioName = ScenarioName;
    NewScenario->Description = Description;
    NewScenario->CreationDate = FDateTime::Now();
    NewScenario->TargetMapName = World->GetName();

    TArray<AActor*> AllLights = GetAllLightsInLevel();
    
    for (AActor* LightActor : AllLights)
    {
        if (!LightActor) continue;

        ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
        if (!LightComp) continue;

        FLightSettings LightSettings;
        
        LightSettings.LightName = LightActor->GetName();
        LightSettings.LightActor = LightActor;
        
        LightSettings.Location = LightActor->GetActorLocation();
        LightSettings.Rotation = LightActor->GetActorRotation();
        LightSettings.Scale = LightActor->GetActorScale3D();

        LightSettings.Intensity = LightComp->Intensity;
        LightSettings.LightColor = LightComp->GetLightColor();
        LightSettings.Temperature = LightComp->Temperature;
        LightSettings.bUseTemperature = LightComp->bUseTemperature;
        LightSettings.VolumetricScatteringIntensity = LightComp->VolumetricScatteringIntensity;
        LightSettings.bAffectsWorld = LightComp->bAffectsWorld;
        LightSettings.bIsEnabled = LightComp->IsVisible();

        LightSettings.bCastShadows = LightComp->CastShadows;
        LightSettings.bCastStaticShadows = LightComp->CastStaticShadows;
        LightSettings.bCastDynamicShadows = LightComp->CastDynamicShadows;
        LightSettings.bCastVolumetricShadow = LightComp->bCastVolumetricShadow;
        LightSettings.bCastDeepShadow = LightComp->bCastDeepShadow;
        LightSettings.bCastShadowsFromCinematicObjectsOnly = LightComp->bCastShadowsFromCinematicObjectsOnly;
        LightSettings.ShadowResolutionScale = LightComp->ShadowResolutionScale;
        LightSettings.ShadowBias = LightComp->ShadowBias;
        LightSettings.ShadowSlopeBias = LightComp->ShadowSlopeBias;
        LightSettings.ShadowSharpen = LightComp->ShadowSharpen;
        LightSettings.ContactShadowLength = LightComp->ContactShadowLength;
        LightSettings.bContactShadowLengthInWS = LightComp->ContactShadowLengthInWS;

        LightSettings.LightFunctionScale = LightComp->LightFunctionScale;
        LightSettings.LightFunctionFadeDistance = LightComp->LightFunctionFadeDistance;
        LightSettings.DisabledBrightness = LightComp->DisabledBrightness;

        LightSettings.bUseIESProfile = LightComp->bUseIESBrightness;
        LightSettings.IESBrightnessScale = LightComp->IESBrightnessScale;

        LightSettings.IndirectLightingIntensity = LightComp->IndirectLightingIntensity;

        LightSettings.bVisible = LightComp->IsVisible();
        LightSettings.bAffectReflection = LightComp->bAffectReflection;
        LightSettings.bAffectGlobalIllumination = LightComp->bAffectGlobalIllumination;
        LightSettings.DeepShadowLayerDistribution = LightComp->DeepShadowLayerDistribution;
        LightSettings.SamplesPerPixel = LightComp->SamplesPerPixel;

        LightSettings.Mobility = (uint8)LightComp->Mobility.GetValue();

        LightSettings.SpecularScale = LightComp->SpecularScale;
        
        if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(LightComp))
        {
            LightSettings.LightType = "PointLight";
            LightSettings.IntensityUnits = (uint8)PointLight->IntensityUnits;
            LightSettings.AttenuationRadius = PointLight->AttenuationRadius;
            LightSettings.bUseInverseSquaredFalloff = PointLight->bUseInverseSquaredFalloff;
            LightSettings.LightFalloffExponent = PointLight->LightFalloffExponent;
            LightSettings.SourceRadius = PointLight->SourceRadius;
            LightSettings.SoftSourceRadius = PointLight->SoftSourceRadius;
            LightSettings.SourceLength = PointLight->SourceLength;
        }
        else if (USpotLightComponent* SpotLight = Cast<USpotLightComponent>(LightComp))
        {
            LightSettings.LightType = "SpotLight";
            LightSettings.IntensityUnits = (uint8)SpotLight->IntensityUnits;
            LightSettings.AttenuationRadius = SpotLight->AttenuationRadius;
            LightSettings.bUseInverseSquaredFalloff = SpotLight->bUseInverseSquaredFalloff;
            LightSettings.LightFalloffExponent = SpotLight->LightFalloffExponent;
            LightSettings.InnerConeAngle = SpotLight->InnerConeAngle;
            LightSettings.OuterConeAngle = SpotLight->OuterConeAngle;
            LightSettings.SourceRadius = SpotLight->SourceRadius;
            LightSettings.SoftSourceRadius = SpotLight->SoftSourceRadius;
            LightSettings.SourceLength = SpotLight->SourceLength;
        }
        else if (URectLightComponent* RectLight = Cast<URectLightComponent>(LightComp))
        {
            LightSettings.LightType = "RectLight";
            LightSettings.IntensityUnits = (uint8)RectLight->IntensityUnits;
            LightSettings.AttenuationRadius = RectLight->AttenuationRadius;
            LightSettings.SourceWidth = RectLight->SourceWidth;
            LightSettings.SourceHeight = RectLight->SourceHeight;
            LightSettings.BarnDoorAngle = RectLight->BarnDoorAngle;
            LightSettings.BarnDoorLength = RectLight->BarnDoorLength;
        }
        else if (UDirectionalLightComponent* DirLight = Cast<UDirectionalLightComponent>(LightComp))
        {
            LightSettings.LightType = "DirectionalLight";
            
            LightSettings.DynamicShadowDistanceMovableLight = DirLight->DynamicShadowDistanceMovableLight;
            LightSettings.DynamicShadowDistanceStationaryLight = DirLight->DynamicShadowDistanceStationaryLight;
            LightSettings.DynamicShadowCascades = DirLight->DynamicShadowCascades;
            LightSettings.CascadeDistributionExponent = DirLight->CascadeDistributionExponent;
            LightSettings.CascadeTransitionFraction = DirLight->CascadeTransitionFraction;
            LightSettings.ShadowDistanceFadeoutFraction = DirLight->ShadowDistanceFadeoutFraction;
            LightSettings.bUseInsetShadowsForMovableObjects = DirLight->bUseInsetShadowsForMovableObjects;
            LightSettings.FarShadowCascadeCount = DirLight->FarShadowCascadeCount;
            LightSettings.FarShadowDistance = DirLight->FarShadowDistance;

            LightSettings.bAtmosphereSunLight = DirLight->bAtmosphereSunLight;
            LightSettings.AtmosphereSunLightIndex = DirLight->AtmosphereSunLightIndex;
            LightSettings.AtmosphereSunDiskColorScale = DirLight->AtmosphereSunDiskColorScale;
            LightSettings.bPerPixelAtmosphereTransmittance = DirLight->bPerPixelAtmosphereTransmittance;
            LightSettings.bCastShadowsOnClouds = DirLight->bCastShadowsOnClouds;
            LightSettings.bCastShadowsOnAtmosphere = DirLight->bCastShadowsOnAtmosphere;
            LightSettings.bCastCloudShadows = DirLight->bCastCloudShadows;
            LightSettings.CloudShadowStrength = DirLight->CloudShadowStrength;
            LightSettings.CloudShadowOnAtmosphereStrength = DirLight->CloudShadowOnAtmosphereStrength;
            LightSettings.CloudShadowOnSurfaceStrength = DirLight->CloudShadowOnSurfaceStrength;
            LightSettings.CloudShadowDepthBias = DirLight->CloudShadowDepthBias;
            LightSettings.CloudShadowExtent = DirLight->CloudShadowExtent;
            LightSettings.CloudShadowMapResolutionScale = DirLight->CloudShadowMapResolutionScale;
            LightSettings.CloudShadowRaySampleCountScale = DirLight->CloudShadowRaySampleCountScale;

            LightSettings.bEnableLightShaftOcclusion = DirLight->bEnableLightShaftOcclusion;
            LightSettings.OcclusionMaskDarkness = DirLight->OcclusionMaskDarkness;
            LightSettings.OcclusionDepthRange = DirLight->OcclusionDepthRange;
            LightSettings.LightShaftOverrideDirection = DirLight->LightShaftOverrideDirection;
        }
        else if (USkyLightComponent* SkyLight = Cast<USkyLightComponent>(LightComp))
        {
            LightSettings.LightType = "SkyLight";
        }

        NewScenario->AllLights.Add(LightSettings);
    }

    NewScenario->FogSettings = CaptureFogSettings();
    NewScenario->SkyLightSettings = CaptureSkyLightSettings();
    NewScenario->SunSettings = CaptureDirectionalLightSettings();
    
    TArray<APostProcessVolume*> PPVolumes = GetAllPostProcessVolumes();
    for (APostProcessVolume* PPVolume : PPVolumes)
    {
        if (PPVolume)
        {
            NewScenario->PostProcessVolumes.Add(CapturePostProcessSettings(PPVolume));
        }
    }

    NewScenario->MarkPackageDirty();

    return NewScenario;
}

void ULightingToolsLibrary::ApplyCompleteScenario(ULightingScenario* Scenario)
{
    if (!Scenario)
    {
        return;
    }

    FScopedTransaction Transaction(FText::FromString("Apply Complete Scenario"));

    if (Scenario->LevelInfo.bAutoLoadLevel)
    {
        switch (Scenario->LevelInfo.LevelType)
        {
            case EScenarioLevelType::SubLevel:
                if (Scenario->LevelInfo.SubLevelToLoad.IsValid())
                {
                    LoadSubLevel(Scenario->LevelInfo.SubLevelToLoad);
                }
                break;

            case EScenarioLevelType::DataLayer:
                if (!Scenario->LevelInfo.DataLayerName.IsNone())
                {
                    ActivateDataLayer(Scenario->LevelInfo.DataLayerName);
                }
                break;

            default:
                break;
        }
    }

    for (const FLightSettings& LightSettings : Scenario->AllLights)
    {
        AActor* LightActor = LightSettings.LightActor.Get();
        if (!LightActor) continue;

        ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
        if (!LightComp) continue;

        LightComp->Modify();

        LightComp->SetIntensity(LightSettings.Intensity);
        LightComp->SetLightColor(LightSettings.LightColor);
        LightComp->SetVisibility(LightSettings.bIsEnabled);
        LightComp->SetCastShadows(LightSettings.bCastShadows);
        LightComp->SetTemperature(LightSettings.Temperature);
        LightComp->bUseTemperature = LightSettings.bUseTemperature;
        
        if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(LightComp))
        {
            PointLight->SetAttenuationRadius(LightSettings.AttenuationRadius);
        }

        LightComp->MarkRenderStateDirty();
    }

    ApplyFogSettings(Scenario->FogSettings);
    ApplySkyLightSettings(Scenario->SkyLightSettings);
    ApplyDirectionalLightSettings(Scenario->SunSettings);

    for (const FPostProcessSettings_Saved& PPSettings : Scenario->PostProcessVolumes)
    {
        APostProcessVolume* PPVolume = Cast<APostProcessVolume>(PPSettings.PostProcessVolume.Get());
        if (PPVolume)
        {
            ApplyPostProcessSettings(PPVolume, PPSettings);
        }
    }
}

bool ULightingToolsLibrary::UpdateExistingScenario(ULightingScenario* Scenario)
{
    if (!Scenario)
    {
        return false;
    }
    
    FScopedTransaction Transaction(FText::FromString("Update Lighting Scenario"));
    
    Scenario->Modify();
    Scenario->AllLights.Empty();
    Scenario->CreationDate = FDateTime::Now();

    TArray<AActor*> AllLights = GetAllLightsInLevel();
    
    for (AActor* LightActor : AllLights)
    {
        if (!LightActor) continue;

        ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
        if (!LightComp) continue;

        FLightSettings LightSettings;
        
        LightSettings.LightName = LightActor->GetName();
        LightSettings.LightActor = LightActor;
        
        LightSettings.Location = LightActor->GetActorLocation();
        LightSettings.Rotation = LightActor->GetActorRotation();
        LightSettings.Scale = LightActor->GetActorScale3D();

        LightSettings.Intensity = LightComp->Intensity;
        LightSettings.LightColor = LightComp->GetLightColor();
        LightSettings.Temperature = LightComp->Temperature;
        LightSettings.bUseTemperature = LightComp->bUseTemperature;
        LightSettings.VolumetricScatteringIntensity = LightComp->VolumetricScatteringIntensity;
        LightSettings.bAffectsWorld = LightComp->bAffectsWorld;
        LightSettings.bIsEnabled = LightComp->IsVisible();

        LightSettings.bCastShadows = LightComp->CastShadows;
        LightSettings.bCastStaticShadows = LightComp->CastStaticShadows;
        LightSettings.bCastDynamicShadows = LightComp->CastDynamicShadows;
        LightSettings.bCastVolumetricShadow = LightComp->bCastVolumetricShadow;
        LightSettings.bCastDeepShadow = LightComp->bCastDeepShadow;
        LightSettings.bCastShadowsFromCinematicObjectsOnly = LightComp->bCastShadowsFromCinematicObjectsOnly;
        LightSettings.ShadowResolutionScale = LightComp->ShadowResolutionScale;
        LightSettings.ShadowBias = LightComp->ShadowBias;
        LightSettings.ShadowSlopeBias = LightComp->ShadowSlopeBias;
        LightSettings.ShadowSharpen = LightComp->ShadowSharpen;
        LightSettings.ContactShadowLength = LightComp->ContactShadowLength;
        LightSettings.bContactShadowLengthInWS = LightComp->ContactShadowLengthInWS;

        LightSettings.LightFunctionScale = LightComp->LightFunctionScale;
        LightSettings.LightFunctionFadeDistance = LightComp->LightFunctionFadeDistance;
        LightSettings.DisabledBrightness = LightComp->DisabledBrightness;

        LightSettings.bUseIESProfile = LightComp->bUseIESBrightness;
        LightSettings.IESBrightnessScale = LightComp->IESBrightnessScale;

        LightSettings.IndirectLightingIntensity = LightComp->IndirectLightingIntensity;

        LightSettings.bVisible = LightComp->IsVisible();
        LightSettings.bAffectReflection = LightComp->bAffectReflection;
        LightSettings.bAffectGlobalIllumination = LightComp->bAffectGlobalIllumination;
        LightSettings.DeepShadowLayerDistribution = LightComp->DeepShadowLayerDistribution;
        LightSettings.SamplesPerPixel = LightComp->SamplesPerPixel;

        LightSettings.Mobility = (uint8)LightComp->Mobility.GetValue();

        LightSettings.SpecularScale = LightComp->SpecularScale;
        
        if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(LightComp))
        {
            LightSettings.LightType = "PointLight";
            LightSettings.IntensityUnits = (uint8)PointLight->IntensityUnits;
            LightSettings.AttenuationRadius = PointLight->AttenuationRadius;
            LightSettings.bUseInverseSquaredFalloff = PointLight->bUseInverseSquaredFalloff;
            LightSettings.LightFalloffExponent = PointLight->LightFalloffExponent;
            LightSettings.SourceRadius = PointLight->SourceRadius;
            LightSettings.SoftSourceRadius = PointLight->SoftSourceRadius;
            LightSettings.SourceLength = PointLight->SourceLength;
        }
        else if (USpotLightComponent* SpotLight = Cast<USpotLightComponent>(LightComp))
        {
            LightSettings.LightType = "SpotLight";
            LightSettings.IntensityUnits = (uint8)SpotLight->IntensityUnits;
            LightSettings.AttenuationRadius = SpotLight->AttenuationRadius;
            LightSettings.bUseInverseSquaredFalloff = SpotLight->bUseInverseSquaredFalloff;
            LightSettings.LightFalloffExponent = SpotLight->LightFalloffExponent;
            LightSettings.InnerConeAngle = SpotLight->InnerConeAngle;
            LightSettings.OuterConeAngle = SpotLight->OuterConeAngle;
            LightSettings.SourceRadius = SpotLight->SourceRadius;
            LightSettings.SoftSourceRadius = SpotLight->SoftSourceRadius;
            LightSettings.SourceLength = SpotLight->SourceLength;
        }
        else if (URectLightComponent* RectLight = Cast<URectLightComponent>(LightComp))
        {
            LightSettings.LightType = "RectLight";
            LightSettings.IntensityUnits = (uint8)RectLight->IntensityUnits;
            LightSettings.AttenuationRadius = RectLight->AttenuationRadius;
            LightSettings.SourceWidth = RectLight->SourceWidth;
            LightSettings.SourceHeight = RectLight->SourceHeight;
            LightSettings.BarnDoorAngle = RectLight->BarnDoorAngle;
            LightSettings.BarnDoorLength = RectLight->BarnDoorLength;
        }
        else if (UDirectionalLightComponent* DirLight = Cast<UDirectionalLightComponent>(LightComp))
        {
            LightSettings.LightType = "DirectionalLight";
            
            LightSettings.DynamicShadowDistanceMovableLight = DirLight->DynamicShadowDistanceMovableLight;
            LightSettings.DynamicShadowDistanceStationaryLight = DirLight->DynamicShadowDistanceStationaryLight;
            LightSettings.DynamicShadowCascades = DirLight->DynamicShadowCascades;
            LightSettings.CascadeDistributionExponent = DirLight->CascadeDistributionExponent;
            LightSettings.CascadeTransitionFraction = DirLight->CascadeTransitionFraction;
            LightSettings.ShadowDistanceFadeoutFraction = DirLight->ShadowDistanceFadeoutFraction;
            LightSettings.bUseInsetShadowsForMovableObjects = DirLight->bUseInsetShadowsForMovableObjects;
            LightSettings.FarShadowCascadeCount = DirLight->FarShadowCascadeCount;
            LightSettings.FarShadowDistance = DirLight->FarShadowDistance;

            LightSettings.bAtmosphereSunLight = DirLight->bAtmosphereSunLight;
            LightSettings.AtmosphereSunLightIndex = DirLight->AtmosphereSunLightIndex;
            LightSettings.AtmosphereSunDiskColorScale = DirLight->AtmosphereSunDiskColorScale;
            LightSettings.bPerPixelAtmosphereTransmittance = DirLight->bPerPixelAtmosphereTransmittance;
            LightSettings.bCastShadowsOnClouds = DirLight->bCastShadowsOnClouds;
            LightSettings.bCastShadowsOnAtmosphere = DirLight->bCastShadowsOnAtmosphere;
            LightSettings.bCastCloudShadows = DirLight->bCastCloudShadows;
            LightSettings.CloudShadowStrength = DirLight->CloudShadowStrength;
            LightSettings.CloudShadowOnAtmosphereStrength = DirLight->CloudShadowOnAtmosphereStrength;
            LightSettings.CloudShadowOnSurfaceStrength = DirLight->CloudShadowOnSurfaceStrength;
            LightSettings.CloudShadowDepthBias = DirLight->CloudShadowDepthBias;
            LightSettings.CloudShadowExtent = DirLight->CloudShadowExtent;
            LightSettings.CloudShadowMapResolutionScale = DirLight->CloudShadowMapResolutionScale;
            LightSettings.CloudShadowRaySampleCountScale = DirLight->CloudShadowRaySampleCountScale;

            LightSettings.bEnableLightShaftOcclusion = DirLight->bEnableLightShaftOcclusion;
            LightSettings.OcclusionMaskDarkness = DirLight->OcclusionMaskDarkness;
            LightSettings.OcclusionDepthRange = DirLight->OcclusionDepthRange;
            LightSettings.LightShaftOverrideDirection = DirLight->LightShaftOverrideDirection;
        }
        else if (USkyLightComponent* SkyLight = Cast<USkyLightComponent>(LightComp))
        {
            LightSettings.LightType = "SkyLight";
        }

        Scenario->AllLights.Add(LightSettings);
    }

    Scenario->FogSettings = CaptureFogSettings();
    Scenario->SkyLightSettings = CaptureSkyLightSettings();
    Scenario->SunSettings = CaptureDirectionalLightSettings();
    
    Scenario->PostProcessVolumes.Empty();
    TArray<APostProcessVolume*> PPVolumes = GetAllPostProcessVolumes();
    for (APostProcessVolume* PPVolume : PPVolumes)
    {
        if (PPVolume)
        {
            Scenario->PostProcessVolumes.Add(CapturePostProcessSettings(PPVolume));
        }
    }

    Scenario->MarkPackageDirty();

    return true;
}

AActor* ULightingToolsLibrary::GetExponentialHeightFog()
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    for (TActorIterator<AExponentialHeightFog> It(World); It; ++It)
    {
        return *It;
    }

    return nullptr;
}

FFogSettings_Saved ULightingToolsLibrary::CaptureFogSettings()
{
    FFogSettings_Saved FogSettings;
    
    AActor* FogActor = GetExponentialHeightFog();
    if (!FogActor)
    {
        return FogSettings;
    }

    AExponentialHeightFog* Fog = Cast<AExponentialHeightFog>(FogActor);
    if (!Fog || !Fog->GetComponent()) return FogSettings;

    UExponentialHeightFogComponent* FogComp = Fog->GetComponent();
    
    FogSettings.bEnabled = FogComp->IsVisible();
    FogSettings.FogDensity = FogComp->FogDensity;
    FogSettings.FogInscatteringColor = FLinearColor::White;
    FogSettings.FogHeightFalloff = FogComp->FogHeightFalloff;
    FogSettings.FogMaxOpacity = FogComp->FogMaxOpacity;
    FogSettings.StartDistance = FogComp->StartDistance;
    FogSettings.DirectionalInscatteringExponent = FogComp->DirectionalInscatteringExponent;
    FogSettings.DirectionalInscatteringColor = FLinearColor::White;

    return FogSettings;
}

void ULightingToolsLibrary::ApplyFogSettings(const FFogSettings_Saved& FogSettings)
{
    AActor* FogActor = GetExponentialHeightFog();
    if (!FogActor)
    {
        return;
    }

    AExponentialHeightFog* Fog = Cast<AExponentialHeightFog>(FogActor);
    if (!Fog || !Fog->GetComponent()) return;

    UExponentialHeightFogComponent* FogComp = Fog->GetComponent();
    
    FogComp->Modify();

    FogComp->SetVisibility(FogSettings.bEnabled);
    FogComp->SetFogDensity(FogSettings.FogDensity);
    FogComp->SetFogHeightFalloff(FogSettings.FogHeightFalloff);
    FogComp->SetFogMaxOpacity(FogSettings.FogMaxOpacity);
    FogComp->SetStartDistance(FogSettings.StartDistance);
    FogComp->SetDirectionalInscatteringExponent(FogSettings.DirectionalInscatteringExponent);
    
    FogComp->MarkRenderStateDirty();
}

AActor* ULightingToolsLibrary::GetSkyLight()
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        if (It->FindComponentByClass<USkyLightComponent>())
        {
            return *It;
        }
    }

    return nullptr;
}

FSkyLightSettings_Saved ULightingToolsLibrary::CaptureSkyLightSettings()
{
    FSkyLightSettings_Saved SkySettings;
    
    AActor* SkyActor = GetSkyLight();
    if (!SkyActor) return SkySettings;

    USkyLightComponent* SkyComp = SkyActor->FindComponentByClass<USkyLightComponent>();
    if (!SkyComp) return SkySettings;

    SkySettings.bEnabled = SkyComp->IsVisible();
    SkySettings.Intensity = SkyComp->Intensity;
    SkySettings.Color = SkyComp->GetLightColor();
    SkySettings.IndirectLightingIntensity = SkyComp->IndirectLightingIntensity;
    SkySettings.VolumetricScatteringIntensity = SkyComp->VolumetricScatteringIntensity;

    return SkySettings;
}

void ULightingToolsLibrary::ApplySkyLightSettings(const FSkyLightSettings_Saved& SkySettings)
{
    AActor* SkyActor = GetSkyLight();
    if (!SkyActor) return;

    USkyLightComponent* SkyComp = SkyActor->FindComponentByClass<USkyLightComponent>();
    if (!SkyComp) return;

    SkyComp->Modify();

    SkyComp->SetVisibility(SkySettings.bEnabled);
    SkyComp->SetIntensity(SkySettings.Intensity);
    SkyComp->SetLightColor(SkySettings.Color);
    SkyComp->SetIndirectLightingIntensity(SkySettings.IndirectLightingIntensity);
    SkyComp->SetVolumetricScatteringIntensity(SkySettings.VolumetricScatteringIntensity);
    
    SkyComp->MarkRenderStateDirty();
}

AActor* ULightingToolsLibrary::GetDirectionalLight()
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    for (TActorIterator<AActor> It(World); It; ++It)
    {
        if (It->FindComponentByClass<UDirectionalLightComponent>())
        {
            return *It;
        }
    }

    return nullptr;
}

FDirectionalLightSettings_Saved ULightingToolsLibrary::CaptureDirectionalLightSettings()
{
    FDirectionalLightSettings_Saved SunSettings;
    
    AActor* SunActor = GetDirectionalLight();
    if (!SunActor) return SunSettings;

    UDirectionalLightComponent* DirLight = SunActor->FindComponentByClass<UDirectionalLightComponent>();
    if (!DirLight) return SunSettings;

    SunSettings.bEnabled = DirLight->IsVisible();
    SunSettings.Intensity = DirLight->Intensity;
    SunSettings.Color = DirLight->GetLightColor();
    SunSettings.Temperature = DirLight->Temperature;
    SunSettings.bUseTemperature = DirLight->bUseTemperature;
    SunSettings.Rotation = SunActor->GetActorRotation();
    SunSettings.bCastShadows = DirLight->CastShadows;
    SunSettings.IndirectLightingIntensity = DirLight->IndirectLightingIntensity;
    SunSettings.VolumetricScatteringIntensity = DirLight->VolumetricScatteringIntensity;

    return SunSettings;
}

void ULightingToolsLibrary::ApplyDirectionalLightSettings(const FDirectionalLightSettings_Saved& SunSettings)
{
    AActor* SunActor = GetDirectionalLight();
    if (!SunActor) return;

    UDirectionalLightComponent* DirLight = SunActor->FindComponentByClass<UDirectionalLightComponent>();
    if (!DirLight) return;

    SunActor->Modify();
    DirLight->Modify();

    DirLight->SetVisibility(SunSettings.bEnabled);
    DirLight->SetIntensity(SunSettings.Intensity);
    DirLight->SetLightColor(SunSettings.Color);
    DirLight->SetTemperature(SunSettings.Temperature);
    DirLight->bUseTemperature = SunSettings.bUseTemperature;
    SunActor->SetActorRotation(SunSettings.Rotation);
    DirLight->SetCastShadows(SunSettings.bCastShadows);
    DirLight->SetIndirectLightingIntensity(SunSettings.IndirectLightingIntensity);
    DirLight->SetVolumetricScatteringIntensity(SunSettings.VolumetricScatteringIntensity);
    
    DirLight->MarkRenderStateDirty();
}

TArray<APostProcessVolume*> ULightingToolsLibrary::GetAllPostProcessVolumes()
{
    TArray<APostProcessVolume*> PPVolumes;
    
    if (!GEditor) return PPVolumes;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return PPVolumes;

    for (TActorIterator<APostProcessVolume> It(World); It; ++It)
    {
        PPVolumes.Add(*It);
    }

    return PPVolumes;
}

FPostProcessSettings_Saved ULightingToolsLibrary::CapturePostProcessSettings(APostProcessVolume* PPVolume)
{
    FPostProcessSettings_Saved SavedSettings;
    
    if (!PPVolume) return SavedSettings;

    SavedSettings.PostProcessName = PPVolume->GetName();
    SavedSettings.PostProcessVolume = PPVolume;
    SavedSettings.bEnabled = PPVolume->bEnabled;
    
    const FPostProcessSettings& Settings = PPVolume->Settings;
    
    SavedSettings.BloomIntensity = Settings.BloomIntensity;
    SavedSettings.Exposure = Settings.AutoExposureBias;
    SavedSettings.Saturation = Settings.ColorSaturation.W;
    SavedSettings.Contrast = Settings.ColorContrast.W;

    return SavedSettings;
}

void ULightingToolsLibrary::ApplyPostProcessSettings(APostProcessVolume* PPVolume, const FPostProcessSettings_Saved& Settings)
{
    if (!PPVolume) return;

    PPVolume->Modify();

    PPVolume->bEnabled = Settings.bEnabled;
    
    PPVolume->Settings.BloomIntensity = Settings.BloomIntensity;
    PPVolume->Settings.AutoExposureBias = Settings.Exposure;
    PPVolume->Settings.ColorSaturation.W = Settings.Saturation;
    PPVolume->Settings.ColorContrast.W = Settings.Contrast;
    
    PPVolume->MarkComponentsRenderStateDirty();
}

void ULightingToolsLibrary::SelectActorInWorld(AActor* Actor)
{
    if (!GEditor || !Actor) return;

    GEditor->SelectNone(true, true);
    GEditor->SelectActor(Actor, true, true);
}

bool ULightingToolsLibrary::IsActorSelected(AActor* Actor)
{
    if (!Actor || !GEditor)
    {
        return false;
    }

    return GEditor->GetSelectedActors()->IsSelected(Actor);
}

void ULightingToolsLibrary::SetLightAttenuationRadius(AActor* LightActor, float Radius)
{
    if (!LightActor)
    {
        return;
    }

    FScopedTransaction Transaction(FText::FromString("Change Light Attenuation Radius"));

    bool bSuccess = false;

    if (UPointLightComponent* PointLight = LightActor->FindComponentByClass<UPointLightComponent>())
    {
        PointLight->Modify();
        
        PointLight->AttenuationRadius = Radius;
        
        float NewRadius = PointLight->AttenuationRadius;
        
        PointLight->UpdateColorAndBrightness();
        PointLight->MarkRenderStateDirty();
        
#if WITH_EDITOR
        FPropertyChangedEvent PropertyChangedEvent(
            FindFieldChecked<FProperty>(UPointLightComponent::StaticClass(), GET_MEMBER_NAME_CHECKED(UPointLightComponent, AttenuationRadius))
        );
        PointLight->PostEditChangeProperty(PropertyChangedEvent);
#endif
        
        bSuccess = true;
    }
    else if (USpotLightComponent* SpotLight = LightActor->FindComponentByClass<USpotLightComponent>())
    {
        SpotLight->Modify();
        SpotLight->AttenuationRadius = Radius;
        SpotLight->UpdateColorAndBrightness();
        SpotLight->MarkRenderStateDirty();
        
#if WITH_EDITOR
        FPropertyChangedEvent PropertyChangedEvent(
            FindFieldChecked<FProperty>(USpotLightComponent::StaticClass(), GET_MEMBER_NAME_CHECKED(USpotLightComponent, AttenuationRadius))
        );
        SpotLight->PostEditChangeProperty(PropertyChangedEvent);
#endif
        
        bSuccess = true;
    }
    else if (URectLightComponent* RectLight = LightActor->FindComponentByClass<URectLightComponent>())
    {
        RectLight->Modify();
        RectLight->AttenuationRadius = Radius;
        RectLight->UpdateColorAndBrightness();
        RectLight->MarkRenderStateDirty();
        
#if WITH_EDITOR
        FPropertyChangedEvent PropertyChangedEvent(
            FindFieldChecked<FProperty>(URectLightComponent::StaticClass(), GET_MEMBER_NAME_CHECKED(URectLightComponent, AttenuationRadius))
        );
        RectLight->PostEditChangeProperty(PropertyChangedEvent);
#endif
        
        bSuccess = true;
    }

    if (GEditor)
    {
        GEditor->RedrawAllViewports();
    }
}

float ULightingToolsLibrary::GetLightAttenuationRadius(AActor* LightActor)
{
    if (!LightActor)
    {
        return 0.0f;
    }
    
    if (UPointLightComponent* PointLight = LightActor->FindComponentByClass<UPointLightComponent>())
    {
        float Radius = PointLight->AttenuationRadius;
        return Radius;
    }

    if (USpotLightComponent* SpotLight = LightActor->FindComponentByClass<USpotLightComponent>())
    {
        float Radius = SpotLight->AttenuationRadius;
        return Radius;
    }

    if (URectLightComponent* RectLight = LightActor->FindComponentByClass<URectLightComponent>())
    {
        float Radius = RectLight->AttenuationRadius;
        return Radius;
    }

    return 0.0f;
}

TArray<ULightingScenario*> ULightingToolsLibrary::GetAllScenarioAssets()
{
    TArray<ULightingScenario*> Scenarios;
    
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
    
    AssetRegistry.SearchAllAssets(true);
    
    TArray<FAssetData> AssetData;
    FTopLevelAssetPath ClassPath = ULightingScenario::StaticClass()->GetClassPathName();
    AssetRegistry.GetAssetsByClass(ClassPath, AssetData, true);
    
    for (const FAssetData& Asset : AssetData)
    {
        FString PackagePath = Asset.PackageName.ToString();
        
        if (PackagePath.StartsWith(TEXT("/Game/")) || 
            PackagePath.StartsWith(TEXT("/LightingArtistTools/")))
        {
            if (ULightingScenario* Scenario = Cast<ULightingScenario>(Asset.GetAsset()))
            {
                Scenarios.Add(Scenario);
            }
        }
    }
    
    return Scenarios;
}

TArray<UColorPaletteAsset*> ULightingToolsLibrary::GetAllColorPalettes()
{
    TArray<UColorPaletteAsset*> Palettes;
    
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
    
    AssetRegistry.SearchAllAssets(true);
    
    TArray<FAssetData> AssetData;
    FTopLevelAssetPath ClassPath = UColorPaletteAsset::StaticClass()->GetClassPathName();
    AssetRegistry.GetAssetsByClass(ClassPath, AssetData, true);
    
    for (const FAssetData& Asset : AssetData)
    {
        FString PackagePath = Asset.PackageName.ToString();
        
        if (PackagePath.StartsWith(TEXT("/Game/")) || 
            PackagePath.StartsWith(TEXT("/LightingArtistTools/")))
        {
            if (UColorPaletteAsset* Palette = Cast<UColorPaletteAsset>(Asset.GetAsset()))
            {
                Palettes.Add(Palette);
            }
        }
    }
    
    return Palettes;
}

UColorPaletteAsset* ULightingToolsLibrary::GetColorPaletteByName(const FString& PaletteName)
{
    TArray<UColorPaletteAsset*> AllPalettes = GetAllColorPalettes();
    
    for (UColorPaletteAsset* Palette : AllPalettes)
    {
        if (Palette && Palette->PaletteName == PaletteName)
        {
            return Palette;
        }
    }
    
    return nullptr;
}

void ULightingToolsLibrary::ApplyColorToSelectedLights(FLinearColor Color, float Temperature)
{
    if (!GEditor) return;

    USelection* SelectedActors = GEditor->GetSelectedActors();
    if (!SelectedActors) return;

    TArray<AActor*> Actors;
    SelectedActors->GetSelectedObjects<AActor>(Actors);

    if (Actors.Num() == 0) return;

    FScopedTransaction Transaction(FText::FromString("Apply Color to Selected Lights"));

    for (AActor* Actor : Actors)
    {
        if (!Actor) continue;

        ULightComponent* LightComp = Actor->FindComponentByClass<ULightComponent>();
        if (LightComp)
        {
            LightComp->Modify();
            LightComp->SetLightColor(Color);
            LightComp->SetTemperature(Temperature);
            LightComp->MarkRenderStateDirty();
        }
    }
}

UColorPaletteAsset* ULightingToolsLibrary::CreateColorPaletteAsset(const FString& PaletteName, const FString& Category)
{
    FString PackagePath = FString::Printf(TEXT("/Game/LightingTools/ColorPalettes/%s"), *PaletteName);
    
    UPackage* Package = CreatePackage(*PackagePath);
    Package->FullyLoad();

    UColorPaletteAsset* NewPalette = NewObject<UColorPaletteAsset>(Package, *PaletteName, RF_Public | RF_Standalone);

    if (NewPalette)
    {
        NewPalette->PaletteName = PaletteName;
        NewPalette->Category = Category;

        FAssetRegistryModule::AssetCreated(NewPalette);
        Package->MarkPackageDirty();
        
        FString PackageFileName = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
        
        FSavePackageArgs SaveArgs;
        SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
        SaveArgs.Error = GError;
        
        UPackage::SavePackage(Package, NewPalette, *PackageFileName, SaveArgs);
    }

    return NewPalette;
}

void ULightingToolsLibrary::GetViewportCameraTransform(FVector& OutLocation, FRotator& OutRotation)
{
    if (!GEditor) return;

    FViewport* ActiveViewport = GEditor->GetActiveViewport();
    if (!ActiveViewport) return;

    FEditorViewportClient* ViewportClient = static_cast<FEditorViewportClient*>(ActiveViewport->GetClient());
    if (!ViewportClient) return;

    OutLocation = ViewportClient->GetViewLocation();
    OutRotation = ViewportClient->GetViewRotation();
}

AActor* ULightingToolsLibrary::SpawnLightAtViewport(TSubclassOf<AActor> LightClass, bool bUseViewportPosition, bool bUseViewportRotation)
{
    if (!GEditor || !LightClass) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    FScopedTransaction Transaction(FText::FromString("Spawn Light at Viewport"));

    FVector SpawnLocation = FVector::ZeroVector;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    FVector ViewportLocation;
    FRotator ViewportRotation;
    GetViewportCameraTransform(ViewportLocation, ViewportRotation);

    if (bUseViewportPosition)
    {
        FVector ForwardVector = ViewportRotation.Vector();
        SpawnLocation = ViewportLocation + (ForwardVector * 300.0f);
    }
    else
    {
        SpawnLocation = FVector(0, 0, 0);
    }

    if (bUseViewportRotation)
    {
        SpawnRotation = ViewportRotation;
    }
    else
    {
        SpawnRotation = FRotator::ZeroRotator;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NewActor = World->SpawnActor<AActor>(LightClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (NewActor)
    {
        SelectActorInWorld(NewActor);
    }

    return NewActor;
}

AActor* ULightingToolsLibrary::SpawnPointLight(FVector Location, FRotator Rotation)
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    FScopedTransaction Transaction(FText::FromString("Spawn Point Light"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    APointLight* PointLight = World->SpawnActor<APointLight>(APointLight::StaticClass(), Location, Rotation, SpawnParams);

    if (PointLight && PointLight->PointLightComponent)
    {
        PointLight->PointLightComponent->SetIntensity(5000.0f);
        PointLight->PointLightComponent->SetLightColor(FLinearColor::White);
        PointLight->PointLightComponent->SetAttenuationRadius(1000.0f);
        PointLight->PointLightComponent->SetCastShadows(true);

        SelectActorInWorld(PointLight);
    }

    return PointLight;
}

AActor* ULightingToolsLibrary::SpawnSpotLight(FVector Location, FRotator Rotation)
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    FScopedTransaction Transaction(FText::FromString("Spawn Spot Light"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ASpotLight* SpotLight = World->SpawnActor<ASpotLight>(ASpotLight::StaticClass(), Location, Rotation, SpawnParams);

    if (SpotLight && SpotLight->SpotLightComponent)
    {
        SpotLight->SpotLightComponent->SetIntensity(5000.0f);
        SpotLight->SpotLightComponent->SetLightColor(FLinearColor::White);
        SpotLight->SpotLightComponent->SetAttenuationRadius(1000.0f);
        SpotLight->SpotLightComponent->SetInnerConeAngle(20.0f);
        SpotLight->SpotLightComponent->SetOuterConeAngle(44.0f);
        SpotLight->SpotLightComponent->SetCastShadows(true);

        SelectActorInWorld(SpotLight);
    }

    return SpotLight;
}

AActor* ULightingToolsLibrary::SpawnDirectionalLight(FVector Location, FRotator Rotation)
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    FScopedTransaction Transaction(FText::FromString("Spawn Directional Light"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ADirectionalLight* DirLight = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), Location, Rotation, SpawnParams);

    if (DirLight && DirLight->GetLightComponent())
    {
        DirLight->GetLightComponent()->SetIntensity(10.0f);
        DirLight->GetLightComponent()->SetLightColor(FLinearColor::White);
        DirLight->GetLightComponent()->SetCastShadows(true);

        SelectActorInWorld(DirLight);
    }

    return DirLight;
}

AActor* ULightingToolsLibrary::SpawnRectLight(FVector Location, FRotator Rotation)
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    FScopedTransaction Transaction(FText::FromString("Spawn Rect Light"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ARectLight* RectLight = World->SpawnActor<ARectLight>(ARectLight::StaticClass(), Location, Rotation, SpawnParams);

    if (RectLight && RectLight->RectLightComponent)
    {
        RectLight->RectLightComponent->SetIntensity(5000.0f);
        RectLight->RectLightComponent->SetLightColor(FLinearColor::White);
        RectLight->RectLightComponent->SetSourceWidth(100.0f);
        RectLight->RectLightComponent->SetSourceHeight(100.0f);
        RectLight->RectLightComponent->SetAttenuationRadius(1000.0f);
        RectLight->RectLightComponent->SetCastShadows(true);

        SelectActorInWorld(RectLight);
    }

    return RectLight;
}

AActor* ULightingToolsLibrary::SpawnExponentialHeightFog(FVector Location)
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    AActor* ExistingFog = GetExponentialHeightFog();
    if (ExistingFog)
    {
        SelectActorInWorld(ExistingFog);
        return ExistingFog;
    }

    FScopedTransaction Transaction(FText::FromString("Spawn Exponential Height Fog"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AExponentialHeightFog* Fog = World->SpawnActor<AExponentialHeightFog>(AExponentialHeightFog::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);

    if (Fog)
    {
        SelectActorInWorld(Fog);
    }

    return Fog;
}

AActor* ULightingToolsLibrary::SpawnSkyLight(FVector Location)
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    AActor* ExistingSky = GetSkyLight();
    if (ExistingSky)
    {
        SelectActorInWorld(ExistingSky);
        return ExistingSky;
    }

    FScopedTransaction Transaction(FText::FromString("Spawn Sky Light"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    ASkyLight* SkyLight = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);

    if (SkyLight && SkyLight->GetLightComponent())
    {
        SkyLight->GetLightComponent()->SetIntensity(1.0f);
        SkyLight->GetLightComponent()->SetLightColor(FLinearColor::White);

        SelectActorInWorld(SkyLight);
    }

    return SkyLight;
}

AActor* ULightingToolsLibrary::SpawnPostProcessVolume(FVector Location)
{
    if (!GEditor) return nullptr;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return nullptr;

    FScopedTransaction Transaction(FText::FromString("Spawn Post Process Volume"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    APostProcessVolume* PPVolume = World->SpawnActor<APostProcessVolume>(APostProcessVolume::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);

    if (PPVolume)
    {
        PPVolume->bUnbound = true;

        SelectActorInWorld(PPVolume);
    }

    return PPVolume;
}

TArray<AActor*> ULightingToolsLibrary::GetSelectedActors()
{
    TArray<AActor*> SelectedActors;
    
    if (!GEditor) return SelectedActors;

    USelection* Selection = GEditor->GetSelectedActors();
    if (!Selection) return SelectedActors;

    for (FSelectionIterator It(*Selection); It; ++It)
    {
        AActor* Actor = Cast<AActor>(*It);
        if (Actor)
        {
            SelectedActors.Add(Actor);
        }
    }

    return SelectedActors;
}

void ULightingToolsLibrary::SnapSelectedToViewport(bool bSnapPosition, bool bSnapRotation)
{
    if (!GEditor)
    {
        return;
    }

    FVector ViewportLocation;
    FRotator ViewportRotation;
    GetViewportCameraTransform(ViewportLocation, ViewportRotation);

    TArray<AActor*> SelectedActors = GetSelectedActors();

    if (SelectedActors.Num() == 0)
    {
        return;
    }

    FScopedTransaction Transaction(FText::FromString("Snap to Viewport"));

    FVector ForwardOffset = ViewportRotation.Vector() * 300.0f;
    FVector TargetLocation = ViewportLocation + ForwardOffset;

    for (AActor* Actor : SelectedActors)
    {
        if (!Actor) continue;

        Actor->Modify();

        if (bSnapPosition)
        {
            Actor->SetActorLocation(TargetLocation);
        }

        if (bSnapRotation)
        {
            Actor->SetActorRotation(ViewportRotation);
        }
    }
}

void ULightingToolsLibrary::AlignSelectedToSurface(float TraceDistance)
{
    if (!GEditor)
    {
        return;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World)
    {
        return;
    }

    TArray<AActor*> SelectedActors = GetSelectedActors();

    if (SelectedActors.Num() == 0)
    {
        return;
    }

    FScopedTransaction Transaction(FText::FromString("Align to Surface"));

    int32 AlignedCount = 0;

    for (AActor* Actor : SelectedActors)
    {
        if (!Actor) continue;

        FVector ActorLocation = Actor->GetActorLocation();
        FVector TraceStart = ActorLocation;
        FVector TraceEnd = ActorLocation - FVector(0, 0, TraceDistance);

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Actor);

        bool bHit = World->LineTraceSingleByChannel(
            HitResult,
            TraceStart,
            TraceEnd,
            ECC_Visibility,
            QueryParams
        );

        if (bHit)
        {
            Actor->Modify();

            FVector NewLocation = HitResult.Location;
            Actor->SetActorLocation(NewLocation);

            if (Actor->FindComponentByClass<ULightComponent>())
            {
                FRotator LightRotation = (-HitResult.Normal).Rotation();
                Actor->SetActorRotation(LightRotation);
            }

            AlignedCount++;

            DrawDebugLine(World, TraceStart, HitResult.Location, FColor::Green, false, 2.0f, 0, 2.0f);
            DrawDebugSphere(World, HitResult.Location, 20.0f, 8, FColor::Red, false, 2.0f);
        }
        else
        {
            DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 2.0f, 0, 2.0f);
        }
    }
}

FString ULightingToolsLibrary::GetCurrentMapName()
{
    if (!GEditor) return FString();

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return FString();

    return World->GetMapName();
}

void ULightingToolsLibrary::LoadSubLevel(TSoftObjectPtr<UWorld> SubLevel)
{
    if (!GEditor || !SubLevel.IsValid()) return;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return;

    FString LevelName = SubLevel.GetAssetName();
    
    for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
    {
        if (StreamingLevel && StreamingLevel->GetWorldAssetPackageName() == LevelName)
        {
            StreamingLevel->SetShouldBeLoaded(true);
            StreamingLevel->SetShouldBeVisible(true);
            
            return;
        }
    }
}

void ULightingToolsLibrary::UnloadSubLevel(TSoftObjectPtr<UWorld> SubLevel)
{
    if (!GEditor || !SubLevel.IsValid()) return;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return;

    FString LevelName = SubLevel.GetAssetName();
    
    for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
    {
        if (StreamingLevel && StreamingLevel->GetWorldAssetPackageName() == LevelName)
        {
            StreamingLevel->SetShouldBeLoaded(false);
            StreamingLevel->SetShouldBeVisible(false);
            
            return;
        }
    }
}

void ULightingToolsLibrary::ActivateDataLayer(FName DataLayerName)
{
    if (!GEditor)
    {
        return;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World)
    {
        return;
    }

    UDataLayerSubsystem* DataLayerSubsystem = World->GetSubsystem<UDataLayerSubsystem>();
    if (!DataLayerSubsystem)
    {
        return;
    }

    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    
    UDataLayerInstance* DataLayerInstance = DataLayerSubsystem->GetDataLayerFromName(DataLayerName);
    if (DataLayerInstance)
    {
        DataLayerSubsystem->SetDataLayerRuntimeState(DataLayerInstance, EDataLayerRuntimeState::Activated);
    }
    else
    {
    }
    
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void ULightingToolsLibrary::DeactivateDataLayer(FName DataLayerName)
{
    if (!GEditor) return;

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World) return;

    UDataLayerSubsystem* DataLayerSubsystem = World->GetSubsystem<UDataLayerSubsystem>();
    if (!DataLayerSubsystem) return;

    PRAGMA_DISABLE_DEPRECATION_WARNINGS
    
    UDataLayerInstance* DataLayerInstance = DataLayerSubsystem->GetDataLayerFromName(DataLayerName);
    if (DataLayerInstance)
    {
        DataLayerSubsystem->SetDataLayerRuntimeState(DataLayerInstance, EDataLayerRuntimeState::Unloaded);
    }
    
    PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

TArray<ULightingScenario*> ULightingToolsLibrary::GetScenariosForCurrentMap()
{
    TArray<ULightingScenario*> FilteredScenarios;
    
    FString CurrentMap = GetCurrentMapName();
    
    TArray<ULightingScenario*> AllScenarios = GetAllScenarioAssets();
    
    for (ULightingScenario* Scenario : AllScenarios)
    {
        if (Scenario)
        {
            if (Scenario->TargetMapName.IsEmpty())
            {
                FilteredScenarios.Add(Scenario);
            }
            else if (Scenario->TargetMapName.Equals(CurrentMap, ESearchCase::IgnoreCase))
            {
                FilteredScenarios.Add(Scenario);
            }
            else
            {
            }
        }
    }
        
    return FilteredScenarios;
}

void ULightingToolsLibrary::RecreateMissingLights(ULightingScenario* Scenario)
{
    if (!Scenario || !GEditor)
    {
        return;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World)
    {
        return;
    }

    FScopedTransaction Transaction(FText::FromString("Recreate Missing Lights"));

    int32 RecreatedCount = 0;
    int32 TotalLights = Scenario->AllLights.Num();

    for (FLightSettings& LightSettings : Scenario->AllLights)
    {
        AActor* ExistingLight = LightSettings.LightActor.Get();

        if (!ExistingLight)
        {
            AActor* NewLight = RecreateLightFromSettings(LightSettings);

            if (NewLight)
            {
                LightSettings.LightActor = NewLight;
                RecreatedCount++;
            }
            else
            {
            }
        }
        else
        {
        }
    }

    if (RecreatedCount > 0)
    {
        Scenario->MarkPackageDirty();
    }
}

AActor* ULightingToolsLibrary::RecreateLightFromSettings(const FLightSettings& LightSettings)
{
    if (!GEditor)
    {
        return nullptr;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (!World)
    {
        return nullptr;
    }

    FVector SpawnLocation = LightSettings.Location;
    FRotator SpawnRotation = LightSettings.Rotation;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(*LightSettings.LightName);
    SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* NewLight = nullptr;
    ULightComponent* LightComp = nullptr;

    if (LightSettings.LightType == "PointLight")
    {
        APointLight* PointLight = World->SpawnActor<APointLight>(APointLight::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        if (PointLight && PointLight->PointLightComponent)
        {
            LightComp = PointLight->PointLightComponent;
            PointLight->PointLightComponent->SetAttenuationRadius(LightSettings.AttenuationRadius);
            NewLight = PointLight;
        }
    }
    else if (LightSettings.LightType == "SpotLight")
    {
        ASpotLight* SpotLight = World->SpawnActor<ASpotLight>(ASpotLight::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        if (SpotLight && SpotLight->SpotLightComponent)
        {
            LightComp = SpotLight->SpotLightComponent;
            SpotLight->SpotLightComponent->SetAttenuationRadius(LightSettings.AttenuationRadius);
            NewLight = SpotLight;
        }
    }
    else if (LightSettings.LightType == "DirectionalLight")
    {
        ADirectionalLight* DirLight = World->SpawnActor<ADirectionalLight>(ADirectionalLight::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        if (DirLight && DirLight->GetLightComponent())
        {
            LightComp = DirLight->GetLightComponent();
            NewLight = DirLight;
        }
    }
    else if (LightSettings.LightType == "RectLight")
    {
        ARectLight* RectLight = World->SpawnActor<ARectLight>(ARectLight::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        if (RectLight && RectLight->RectLightComponent)
        {
            LightComp = RectLight->RectLightComponent;
            RectLight->RectLightComponent->SetAttenuationRadius(LightSettings.AttenuationRadius);
            NewLight = RectLight;
        }
    }
    else if (LightSettings.LightType == "SkyLight")
    {
        ASkyLight* SkyLight = World->SpawnActor<ASkyLight>(ASkyLight::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        if (SkyLight && SkyLight->GetLightComponent())
        {
            USkyLightComponent* SkyComp = SkyLight->GetLightComponent();
            
            SkyComp->SetIntensity(LightSettings.Intensity);
            SkyComp->SetLightColor(LightSettings.LightColor);
            SkyComp->SetVisibility(LightSettings.bIsEnabled);
            SkyComp->MarkRenderStateDirty();
            
            NewLight = SkyLight;
        }
    }
    else
    {
        return nullptr;
    }

    if (LightComp)
    {
        LightComp->SetIntensity(LightSettings.Intensity);
        LightComp->SetLightColor(LightSettings.LightColor);
        LightComp->SetVisibility(LightSettings.bIsEnabled);
        LightComp->SetCastShadows(LightSettings.bCastShadows);
        LightComp->SetTemperature(LightSettings.Temperature);
        LightComp->bUseTemperature = LightSettings.bUseTemperature;
        LightComp->MarkRenderStateDirty();
    }

    if (NewLight)
    {
        NewLight->SetActorScale3D(LightSettings.Scale);
        SelectActorInWorld(NewLight);
    }

    return NewLight;
}

int32 ULightingToolsLibrary::CountMissingLights(ULightingScenario* Scenario)
{
    if (!Scenario) return 0;

    int32 MissingCount = 0;

    for (const FLightSettings& LightSettings : Scenario->AllLights)
    {
        if (!LightSettings.LightActor.Get())
        {
            MissingCount++;
        }
    }

    return MissingCount;
}

void ULightingToolsLibrary::SetViewMode(const FString& ViewModeName)
{
    if (!GEditor)
    {
        return;
    }

    FEditorViewportClient* ViewportClient = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
    if (!ViewportClient)
    {
        return;
    }

    EViewModeIndex ViewMode = VMI_Lit;

    if (ViewModeName.Equals(TEXT("Lit"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_Lit;
    }
    else if (ViewModeName.Equals(TEXT("Unlit"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_Unlit;
    }
    else if (ViewModeName.Equals(TEXT("Wireframe"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_BrushWireframe;
    }
    else if (ViewModeName.Equals(TEXT("Lighting Only"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_LightingOnly;
    }
    else if (ViewModeName.Equals(TEXT("Detail Lighting"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_Lit_DetailLighting;
    }
    else if (ViewModeName.Equals(TEXT("Reflections"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_ReflectionOverride;
    }
    else if (ViewModeName.Equals(TEXT("Light Complexity"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_LightComplexity;
    }
    else if (ViewModeName.Equals(TEXT("Shader Complexity"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_ShaderComplexity;
    }
    else if (ViewModeName.Equals(TEXT("Lightmap Density"), ESearchCase::IgnoreCase))
    {
        ViewMode = VMI_LightmapDensity;
    }
    else
    {
        return;
    }

    ViewportClient->SetViewMode(ViewMode);
    ViewportClient->Invalidate();
}

void ULightingToolsLibrary::OpenGPUProfiler()
{
    if (!GEditor)
    {
        return;
    }

    GEditor->Exec(GEditor->GetEditorWorldContext().World(), TEXT("stat gpu"));
}

void ULightingToolsLibrary::ToggleStatUnit()
{
    if (!GEditor)
    {
        return;
    }

    GEditor->Exec(GEditor->GetEditorWorldContext().World(), TEXT("stat unit"));
}

void ULightingToolsLibrary::ToggleStatFPS()
{
    if (!GEditor)
    {
        return;
    }

    GEditor->Exec(GEditor->GetEditorWorldContext().World(), TEXT("stat fps"));
}

void ULightingToolsLibrary::GetLightStatistics(
    int32& OutTotalLights,
    int32& OutShadowCastingLights,
    int32& OutStaticLights,
    int32& OutStationaryLights,
    int32& OutMovableLights)
{
    OutTotalLights = 0;
    OutShadowCastingLights = 0;
    OutStaticLights = 0;
    OutStationaryLights = 0;
    OutMovableLights = 0;

    TArray<AActor*> AllLights = GetAllLightsInLevel();
    OutTotalLights = AllLights.Num();

    for (AActor* LightActor : AllLights)
    {
        if (!LightActor) continue;

        ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
        if (!LightComp) continue;

        if (LightComp->CastShadows)
        {
            OutShadowCastingLights++;
        }

        if (LightComp->Mobility == EComponentMobility::Static)
        {
            OutStaticLights++;
        }
        else if (LightComp->Mobility == EComponentMobility::Stationary)
        {
            OutStationaryLights++;
        }
        else if (LightComp->Mobility == EComponentMobility::Movable)
        {
            OutMovableLights++;
        }
    }
}

void ULightingToolsLibrary::ConvertLightsToPhysicalUnits(const TArray<AActor*>& Lights)
{
    if (Lights.Num() == 0) return;

    FScopedTransaction Transaction(FText::FromString("Convert Lights to Physical Units"));

    int32 ConvertedCount = 0;

    for (AActor* LightActor : Lights)
    {
        if (!LightActor) continue;

        ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
        if (!LightComp) continue;

        LightComp->Modify();

        if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(LightComp))
        {
            PointLight->IntensityUnits = ELightUnits::Lumens;
            float NewIntensity = PointLight->Intensity * 1.7f;
            PointLight->SetIntensity(NewIntensity);
            ConvertedCount++;
        }
        else if (USpotLightComponent* SpotLight = Cast<USpotLightComponent>(LightComp))
        {
            SpotLight->IntensityUnits = ELightUnits::Lumens;
            float NewIntensity = SpotLight->Intensity * 1.7f;
            SpotLight->SetIntensity(NewIntensity);
            ConvertedCount++;
        }
        else if (URectLightComponent* RectLight = Cast<URectLightComponent>(LightComp))
        {
            RectLight->IntensityUnits = ELightUnits::Lumens;
            float NewIntensity = RectLight->Intensity * 1.7f;
            RectLight->SetIntensity(NewIntensity);
            ConvertedCount++;
        }

        LightComp->MarkRenderStateDirty();
    }
}

void ULightingToolsLibrary::OptimizeShadowSettings(const TArray<AActor*>& Lights)
{
    if (Lights.Num() == 0) return;

    FScopedTransaction Transaction(FText::FromString("Optimize Shadow Settings"));

    int32 OptimizedCount = 0;

    for (AActor* LightActor : Lights)
    {
        if (!LightActor) continue;

        ULightComponent* LightComp = LightActor->FindComponentByClass<ULightComponent>();
        if (!LightComp) continue;

        LightComp->Modify();

        if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(LightComp))
        {
            PointLight->ShadowResolutionScale = 0.5f;
            
            if (PointLight->AttenuationRadius > 5000.0f)
            {
                PointLight->SetAttenuationRadius(5000.0f);
            }
        }
        else if (USpotLightComponent* SpotLight = Cast<USpotLightComponent>(LightComp))
        {
            SpotLight->ShadowResolutionScale = 0.5f;
        }

        if (LightComp->Intensity < 500.0f)
        {
            LightComp->SetCastShadows(false);
        }

        LightComp->MarkRenderStateDirty();
        OptimizedCount++;
    }
}