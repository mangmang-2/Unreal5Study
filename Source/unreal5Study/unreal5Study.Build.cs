// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class unreal5Study : ModuleRules
{
	public unreal5Study(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "unreal5Study" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", 
            "SlateCore", "NavigationSystem", "Niagara", "AIModule", "LevelSequence" , "MovieScene", "MotionWarping", "ProceduralMeshComponent",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"});

        PrivateDependencyModuleNames.AddRange(new string[] { });
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
