// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class YMobaGame : ModuleRules
{
	public YMobaGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"NavigationSystem", 
			"AIModule", 
			"SimpleScreenMove",
			"UMG",
			"Landscape"
		});

		//server 需加载的模块.
		if (Target.bWithServerCode == true)
		{
			PublicDependencyModuleNames.AddRange(new string[] {
			});
		}
		//client 需额外加载的模块.
		else
		{
			PublicDependencyModuleNames.AddRange(new string[] {
			"ImageWrapper",
			});
		}

	}
}
