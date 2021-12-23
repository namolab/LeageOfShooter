// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LeageOfShooter : ModuleRules
{
	public LeageOfShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","UMG","AIModule","NavigationSystem", "OnlineSubsystem", "OnlineSubsystemSteam" });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}
