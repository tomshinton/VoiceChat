// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class VoiceChatTarget : TargetRules
{
    public VoiceChatTarget(TargetInfo Target)
    {
        Type = TargetType.Game;
        bUsesSteam = true;

    }

    //
    // TargetRules interface.
    //

    public override void SetupBinaries(
        TargetInfo Target,
        ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
        ref List<string> OutExtraModuleNames
        )
    {
        OutExtraModuleNames.Add("VoiceChat");
    }
}