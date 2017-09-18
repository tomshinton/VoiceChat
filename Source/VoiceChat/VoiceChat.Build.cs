// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VoiceChat : ModuleRules
{
    public VoiceChat(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
    }
}