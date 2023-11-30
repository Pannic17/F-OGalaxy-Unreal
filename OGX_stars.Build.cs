// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OGX_stars : ModuleRules
{
	public OGX_stars(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "HTTP", "Json", "JsonUtilities" });
		// PublicDependencyModuleNames.AddRange(new string[] { "Http", "Json", "JsonUtilities" });
	}
}
