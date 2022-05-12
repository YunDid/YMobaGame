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

		//server ����ص�ģ��.
		if (Target.bWithServerCode == true)
		{
			PublicDependencyModuleNames.AddRange(new string[] {
			});
		}
		//client �������ص�ģ��.
		else
		{
			PublicDependencyModuleNames.AddRange(new string[] {
			"ImageWrapper",
			});
		}

	}
}
