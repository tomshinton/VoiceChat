// Fill out your copyright notice in the Description page of Project Settings.

#include "VoiceChat.h"
#include "VC_PlayerController.h"




AVC_PlayerController::AVC_PlayerController()
{
	bReplicates = true;
}

void AVC_PlayerController::BeginPlay()
{

	Super::BeginPlay();
	//Setup overlap delegate
	AVoiceChatCharacter* ControlledPawn = Cast<AVoiceChatCharacter>(GetPawn());
	if (ControlledPawn && ControlledPawn->RangeComp)
	{
		ControlledPawn->RangeComp->OnComponentBeginOverlap.AddDynamic(this, &AVC_PlayerController::AddToPlayerComms);
		ControlledPawn->RangeComp->OnComponentEndOverlap.AddDynamic(this, &AVC_PlayerController::RemovePlayerFromComms);
	}
}

void AVC_PlayerController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);
}

void AVC_PlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(AVC_PlayerController, NearbyPlayers);
}

void AVC_PlayerController::AddToPlayerComms(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVoiceChatCharacter* OverlappedActor = Cast<AVoiceChatCharacter>(OtherActor);
	if (OverlappedActor && Cast<USphereComponent>(OtherComp))
	{
		Server_AddPlayerToComms(OtherActor);
	}
}

void AVC_PlayerController::Server_AddPlayerToComms_Implementation(AActor * OverlappedActor)
{
	ACharacter* OverlappedChar = Cast<ACharacter>(OverlappedActor);
	if (OverlappedActor)
	{
		APlayerController* OverlappedController = Cast<APlayerController>(OverlappedChar->GetController());
		if (OverlappedController)
		{
			NearbyPlayers.AddUnique(OverlappedController->PlayerState->PlayerName);
			ClientUnmutePlayer(OverlappedController->PlayerState->UniqueId);
			newNotification(OverlappedController->PlayerState->PlayerName + " has entered comms range");
		}
	
		if (NearbyPlayers.Num() > 0)
		{
			Client_EnableVoice(true);
		}
	}
}

bool AVC_PlayerController::Server_AddPlayerToComms_Validate(AActor* OverlappedActor)
{
	return true;
}

void AVC_PlayerController::RemovePlayerFromComms(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVoiceChatCharacter* OverlappedActor = Cast<AVoiceChatCharacter>(OtherActor);
	if (OverlappedActor && Cast<USphereComponent>(OtherComp))
	{
		Server_RemoveControllerFromComms(OtherActor);
	}
}

void AVC_PlayerController::Server_RemoveControllerFromComms_Implementation(AActor* LeftActor)
{
	ACharacter* LeavingActor = Cast<ACharacter>(LeftActor);
	if (LeavingActor)
	{
		APlayerController* LeftController = Cast<APlayerController>(LeavingActor->GetController());
		if (LeftController)
		{
			NearbyPlayers.Remove(LeftController->PlayerState->PlayerName);
			ClientMutePlayer(LeftController->PlayerState->UniqueId);
			newNotification(LeftController->PlayerState->PlayerName + " has left comms range");
		}	


		if (NearbyPlayers.Num() <= 0)
		{
			Client_EnableVoice(false);
		}
	}
}

bool AVC_PlayerController::Server_RemoveControllerFromComms_Validate(AActor* LeftActor)
{
	return true;
}

void AVC_PlayerController::Client_EnableVoice_Implementation(bool bNewVoice)
{
	ToggleSpeaking(bNewVoice);
}

