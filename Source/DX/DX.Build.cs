// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DX : ModuleRules
{
	public DX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Allow existing includes such as "Animation/DXAnimInstanceBase.h" from this module root.
		PublicIncludePaths.AddRange(new string[]
		{
			ModuleDirectory
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput"
		});

		// TestActor.h is included only from DX .cpp files, so Test is a private dependency.
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Test"
		});
	}
}
