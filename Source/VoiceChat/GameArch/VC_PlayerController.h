// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "VoiceChatCharacter.h"
#include "VoiceChatGameMode.h"
#include "GameFramework/PlayerMuteList.h"
#include "VC_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VOICECHAT_API AVC_PlayerController : public APlayerController
{
	GENERATED_BODY()

		AVC_PlayerController();

	void BeginPlay();

	void Possess(APawn* aPawn);

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<FString> NearbyPlayers;

	UPROPERTY()
	TArray<APlayerController*> NearbyControllers;

	UFUNCTION()
	void AddToPlayerComms(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void RemovePlayerFromComms(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddPlayerToComms(AActor* OverlappedActor);
	void Server_AddPlayerToComms_Implementation(AActor* OverlappedActor);
	bool Server_AddPlayerToComms_Validate(AActor* OverlappedActor);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveControllerFromComms(AActor* LeftActor);
	void Server_RemoveControllerFromComms_Implementation(AActor* LeftActor);
	bool Server_RemoveControllerFromComms_Validate(AActor* LeftActor);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Networking | Notifications")
	void newNotification(const FString& newNotification);
	
	UFUNCTION(Client, Reliable)
	void Client_EnableVoice(bool bNewVoice);
};
