// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "GameArch/VC_PlayerController.h"
#include "VoiceChatGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerJoined, FString, NotificationText);

UCLASS(minimalapi)
class AVoiceChatGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVoiceChatGameMode();

	void PostLogin(APlayerController* NewPlayer);

	UPROPERTY(BlueprintAssignable, Category = "Notifications")
	FOnPlayerJoined onPlayerJoin;
};



