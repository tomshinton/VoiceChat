// Fill out your copyright notice in the Description page of Project Settings.

#include "VoiceChat.h"
#include "VC_PlayerController.h"

AVC_PlayerController::AVC_PlayerController()
{
	bReplicates = true;
}

void AVC_PlayerController::StartTalking()
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get(FName("Steam"));

	if (OnlineSub)
	{
	
	}
}

