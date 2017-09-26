// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VoiceChat.h"
#include "VoiceChatGameMode.h"
#include "VoiceChatCharacter.h"


AVoiceChatGameMode::AVoiceChatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	

}

void AVoiceChatGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	TArray<AActor*> TotalPlayers;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVC_PlayerController::StaticClass(), TotalPlayers);

	for (AActor* PC : TotalPlayers)
	{
		AVC_PlayerController* currPC = Cast<AVC_PlayerController>(PC);
		if (PC && PC != NewPlayer)
		{
			FString NotificationText = NewPlayer->PlayerState->PlayerName + " has joined the game";
			onPlayerJoin.Broadcast(NotificationText);
			Cast<AVC_PlayerController>(PC)->newNotification(NotificationText);
		}
	}



}
