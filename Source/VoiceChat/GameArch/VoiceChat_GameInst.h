// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "VC_PlayerController.h"
#include "FindSessionsCallbackProxy.h"
#include "VoiceChat_GameInst.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBuildLobbyList);

USTRUCT(BlueprintType)
struct FBlueprintSessionWrapper
{
	GENERATED_BODY()

	FOnlineSessionSearchResult Session_actual;

	UPROPERTY(BlueprintReadWrite, Category = "Session Blueprint Wrapper")
	FString SessionName;

	UPROPERTY(BlueprintReadWrite, Category = "Session Blueprint Wrapper")
	FString SessionMap;

	UPROPERTY(BlueprintReadWrite, Category = "Session Blueprint Wrapper")
	int32 SessionPing;

	UPROPERTY(BlueprintReadWrite, Category = "Session Blueprint Wrapper")
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite, Category = "Session Blueprint Wrapper")
	int32 CurrPlayers;

};

UCLASS()
class VOICECHAT_API UVoiceChat_GameInst : public UGameInstance
{
	GENERATED_BODY()

		UVoiceChat_GameInst(const FObjectInitializer& ObjectInitializer);

	void Init();

#pragma region BlueprintCallables

	UFUNCTION(BlueprintCallable, Category = "")
		void StartOnlineGame();
	UFUNCTION(BlueprintCallable, Category = "")
		void FindSessions();
	UFUNCTION(BlueprintCallable, Category = "")
		void JoinSession();
	UFUNCTION(BlueprintCallable, Category = "")
		void DestroySession();

	UFUNCTION(BlueprintCallable, Category = "Netowkring | Steam")
		void StartSteamGame();

	UFUNCTION(BlueprintCallable, Category = "Networking | Steam")
		void FindSteamGames();

	UFUNCTION(BlueprintCallable, Category = "Networking | Lobby")
		void JoinSessionFromLobby(FBlueprintSessionWrapper inSession);

	UFUNCTION(BlueprintCallable, Category = "Networking | Lobby")
		TArray<FBlueprintSessionWrapper> BuildLobbysideData();


#pragma endregion

#pragma region CreateSession
	/*Creating a session
	user - User that created the session
	SessionName - name of the "room"
	bIsLan - are we using the internet, or is this a LAN game?
	bIsPresence - is this a "presence" session?
	maxnum - how many players can this session support?*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
#pragma endregion

#pragma region FindSession
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);



	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	void OnFindSessionsComplete(bool bWasSuccessful);
#pragma endregion

#pragma region JoinSession

	virtual bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

#pragma endregion

	
#pragma region DestroySession

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

#pragma endregion


	UPROPERTY(BlueprintAssignable, Category = "Networking | Lobby")
	FOnBuildLobbyList onBuildLobbyList;

};
