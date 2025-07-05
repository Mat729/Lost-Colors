// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectColor : ModuleRules
{
	public ProjectColor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"UMG",
			"Niagara", 
			"NiagaraCore"
		]);
		
		PrivateDependencyModuleNames.AddRange([
			"RenderCore",
			"RHI"
		]);
	}
}
