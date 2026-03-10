// Copyright Maxime Freteau. All Rights Reserved.

#include "DefaultColorPalettes.h"
#include "LightingToolsLibraries.h"

void UDefaultColorPalettes::CreateDefaultPalettes()
{
    CreateCinematicPalette();
    CreateNaturalLightingPalette();
    CreateStudioLightingPalette();
    CreateFantasyPalette();
    CreateHorrorPalette();
    CreateSciFiPalette();
}

UColorPaletteAsset* UDefaultColorPalettes::CreateCinematicPalette()
{
    UColorPaletteAsset* Palette = ULightingToolsLibrary::CreateColorPaletteAsset("Cinematic", "Film");
    
    if (Palette)
    {
        Palette->Description = "Classic cinematic lighting colors";
        
        FColorPreset WarmKey;
        WarmKey.ColorName = "Warm Key Light";
        WarmKey.Color = FLinearColor(1.0f, 0.9f, 0.7f);
        WarmKey.Temperature = 3200.0f;
        WarmKey.Description = "Warm tungsten key light";
        Palette->Colors.Add(WarmKey);
        
        FColorPreset CoolFill;
        CoolFill.ColorName = "Cool Fill";
        CoolFill.Color = FLinearColor(0.7f, 0.8f, 1.0f);
        CoolFill.Temperature = 7000.0f;
        CoolFill.Description = "Cool daylight fill";
        Palette->Colors.Add(CoolFill);
        
        FColorPreset GoldenHour;
        GoldenHour.ColorName = "Golden Hour";
        GoldenHour.Color = FLinearColor(1.0f, 0.8f, 0.5f);
        GoldenHour.Temperature = 3000.0f;
        GoldenHour.Description = "Sunset/sunrise golden light";
        Palette->Colors.Add(GoldenHour);
        
        FColorPreset BlueHour;
        BlueHour.ColorName = "Blue Hour";
        BlueHour.Color = FLinearColor(0.4f, 0.5f, 0.8f);
        BlueHour.Temperature = 8000.0f;
        BlueHour.Description = "Twilight blue ambient";
        Palette->Colors.Add(BlueHour);
        
        FColorPreset RimLight;
        RimLight.ColorName = "Rim Light";
        RimLight.Color = FLinearColor(0.9f, 0.95f, 1.0f);
        RimLight.Temperature = 6500.0f;
        RimLight.Description = "Edge/rim separation light";
        Palette->Colors.Add(RimLight);
        
        Palette->MarkPackageDirty();
    }
    
    return Palette;
}

UColorPaletteAsset* UDefaultColorPalettes::CreateNaturalLightingPalette()
{
    UColorPaletteAsset* Palette = ULightingToolsLibrary::CreateColorPaletteAsset("Natural Lighting", "Nature");
    
    if (Palette)
    {
        Palette->Description = "Natural outdoor lighting conditions";
        
        FColorPreset MidDay;
        MidDay.ColorName = "Midday Sun";
        MidDay.Color = FLinearColor(1.0f, 1.0f, 0.95f);
        MidDay.Temperature = 5500.0f;
        MidDay.Description = "Bright midday sunlight";
        Palette->Colors.Add(MidDay);
        
        FColorPreset Overcast;
        Overcast.ColorName = "Overcast Sky";
        Overcast.Color = FLinearColor(0.8f, 0.85f, 0.9f);
        Overcast.Temperature = 6500.0f;
        Overcast.Description = "Cloudy day diffuse light";
        Palette->Colors.Add(Overcast);
        
        FColorPreset Moonlight;
        Moonlight.ColorName = "Moonlight";
        Moonlight.Color = FLinearColor(0.6f, 0.7f, 0.9f);
        Moonlight.Temperature = 4000.0f;
        Moonlight.Description = "Cool night moonlight";
        Palette->Colors.Add(Moonlight);
        
        FColorPreset Forest;
        Forest.ColorName = "Forest Ambient";
        Forest.Color = FLinearColor(0.5f, 0.7f, 0.5f);
        Forest.Temperature = 5000.0f;
        Forest.Description = "Green forest bounce light";
        Palette->Colors.Add(Forest);
        
        Palette->MarkPackageDirty();
    }
    
    return Palette;
}

UColorPaletteAsset* UDefaultColorPalettes::CreateStudioLightingPalette()
{
    UColorPaletteAsset* Palette = ULightingToolsLibrary::CreateColorPaletteAsset("Studio Lighting", "Studio");
    
    if (Palette)
    {
        Palette->Description = "Professional studio lighting setup";
        
        FColorPreset NeutralWhite;
        NeutralWhite.ColorName = "Neutral White";
        NeutralWhite.Color = FLinearColor::White;
        NeutralWhite.Temperature = 5600.0f;
        NeutralWhite.Description = "Standard daylight balanced";
        Palette->Colors.Add(NeutralWhite);
        
        FColorPreset Tungsten;
        Tungsten.ColorName = "Tungsten 3200K";
        Tungsten.Color = FLinearColor(1.0f, 0.85f, 0.6f);
        Tungsten.Temperature = 3200.0f;
        Tungsten.Description = "Classic tungsten studio light";
        Palette->Colors.Add(Tungsten);
        
        FColorPreset HMI;
        HMI.ColorName = "HMI Daylight";
        HMI.Color = FLinearColor(0.95f, 0.98f, 1.0f);
        HMI.Temperature = 5600.0f;
        HMI.Description = "HMI/daylight balanced";
        Palette->Colors.Add(HMI);
        
        Palette->MarkPackageDirty();
    }
    
    return Palette;
}

UColorPaletteAsset* UDefaultColorPalettes::CreateFantasyPalette()
{
    UColorPaletteAsset* Palette = ULightingToolsLibrary::CreateColorPaletteAsset("Fantasy", "Stylized");
    
    if (Palette)
    {
        Palette->Description = "Magical and fantasy lighting colors";
        
        FColorPreset MagicPurple;
        MagicPurple.ColorName = "Arcane Purple";
        MagicPurple.Color = FLinearColor(0.7f, 0.3f, 1.0f);
        MagicPurple.Temperature = 6500.0f;
        MagicPurple.Description = "Magical purple glow";
        Palette->Colors.Add(MagicPurple);
        
        FColorPreset CrystalBlue;
        CrystalBlue.ColorName = "Crystal Blue";
        CrystalBlue.Color = FLinearColor(0.3f, 0.7f, 1.0f);
        CrystalBlue.Temperature = 8000.0f;
        CrystalBlue.Description = "Mystical blue crystal light";
        Palette->Colors.Add(CrystalBlue);
        
        FColorPreset FireOrange;
        FireOrange.ColorName = "Dragon Fire";
        FireOrange.Color = FLinearColor(1.0f, 0.5f, 0.1f);
        FireOrange.Temperature = 2000.0f;
        FireOrange.Description = "Intense fire/lava glow";
        Palette->Colors.Add(FireOrange);
        
        FColorPreset FairyGreen;
        FairyGreen.ColorName = "Fairy Glow";
        FairyGreen.Color = FLinearColor(0.4f, 1.0f, 0.6f);
        FairyGreen.Temperature = 5500.0f;
        FairyGreen.Description = "Ethereal fairy light";
        Palette->Colors.Add(FairyGreen);
        
        Palette->MarkPackageDirty();
    }
    
    return Palette;
}

UColorPaletteAsset* UDefaultColorPalettes::CreateHorrorPalette()
{
    UColorPaletteAsset* Palette = ULightingToolsLibrary::CreateColorPaletteAsset("Horror", "Dark");
    
    if (Palette)
    {
        Palette->Description = "Dark and unsettling lighting for horror";
        
        FColorPreset SicklyGreen;
        SicklyGreen.ColorName = "Sickly Green";
        SicklyGreen.Color = FLinearColor(0.4f, 0.6f, 0.3f);
        SicklyGreen.Temperature = 4500.0f;
        SicklyGreen.Description = "Unnatural green glow";
        Palette->Colors.Add(SicklyGreen);
        
        FColorPreset BloodRed;
        BloodRed.ColorName = "Blood Red";
        BloodRed.Color = FLinearColor(0.8f, 0.1f, 0.1f);
        BloodRed.Temperature = 2500.0f;
        BloodRed.Description = "Intense red danger light";
        Palette->Colors.Add(BloodRed);
        
        FColorPreset PaleBlue;
        PaleBlue.ColorName = "Ghostly Blue";
        PaleBlue.Color = FLinearColor(0.5f, 0.6f, 0.8f);
        PaleBlue.Temperature = 7500.0f;
        PaleBlue.Description = "Cold ghostly presence";
        Palette->Colors.Add(PaleBlue);
        
        FColorPreset DimYellow;
        DimYellow.ColorName = "Flickering Candle";
        DimYellow.Color = FLinearColor(0.9f, 0.7f, 0.3f);
        DimYellow.Temperature = 2200.0f;
        DimYellow.Description = "Weak candlelight";
        Palette->Colors.Add(DimYellow);
        
        Palette->MarkPackageDirty();
    }
    
    return Palette;
}

UColorPaletteAsset* UDefaultColorPalettes::CreateSciFiPalette()
{
    UColorPaletteAsset* Palette = ULightingToolsLibrary::CreateColorPaletteAsset("Sci-Fi", "Futuristic");
    
    if (Palette)
    {
        Palette->Description = "Futuristic sci-fi lighting";
        
        FColorPreset NeonCyan;
        NeonCyan.ColorName = "Neon Cyan";
        NeonCyan.Color = FLinearColor(0.0f, 1.0f, 1.0f);
        NeonCyan.Temperature = 9000.0f;
        NeonCyan.Description = "Bright neon cyan";
        Palette->Colors.Add(NeonCyan);
        
        FColorPreset ElectricBlue;
        ElectricBlue.ColorName = "Electric Blue";
        ElectricBlue.Color = FLinearColor(0.2f, 0.5f, 1.0f);
        ElectricBlue.Temperature = 10000.0f;
        ElectricBlue.Description = "Electric energy blue";
        Palette->Colors.Add(ElectricBlue);
        
        FColorPreset PlasmaPink;
        PlasmaPink.ColorName = "Plasma Pink";
        PlasmaPink.Color = FLinearColor(1.0f, 0.2f, 0.8f);
        PlasmaPink.Temperature = 6500.0f;
        PlasmaPink.Description = "Plasma energy pink";
        Palette->Colors.Add(PlasmaPink);
        
        FColorPreset HologramBlue;
        HologramBlue.ColorName = "Hologram";
        HologramBlue.Color = FLinearColor(0.4f, 0.8f, 1.0f);
        HologramBlue.Temperature = 8500.0f;
        HologramBlue.Description = "Holographic display blue";
        Palette->Colors.Add(HologramBlue);
        
        Palette->MarkPackageDirty();
    }
    
    return Palette;
}