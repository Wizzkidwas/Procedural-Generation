// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ProceduralGeneration : ModuleRules
{
    // Returns the file path of the game's directory
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    // Returns the file path of third party modules within the game's directory
    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    // Constructor
    public ProceduralGeneration(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Need to add "ProceduralMeshComponent" here
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "ProceduralMeshComponent" });

        // Loads the LibNoise library as a static library
        LoadLib(Target, "LibNoise", "LIB_NOISE");
    }

    // Function to load static libraries
    public bool LoadLib(ReadOnlyTargetRules Target, string libPath, string libName)
    {
        bool isLibrarySupported = false;

        // Supports Win64 and Win32
        if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
        {
            isLibrarySupported = true;
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, libPath, "Libraries");

            // Adds the requested library to the additional libraries list
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, libPath + "." + PlatformString + ".lib"));
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, libPath, "Includes"));
        }
        Definitions.Add(string.Format("WITH_" + libName + "_BINDING={0}", isLibrarySupported ? 1 : 0));
        return isLibrarySupported;
    }
}
