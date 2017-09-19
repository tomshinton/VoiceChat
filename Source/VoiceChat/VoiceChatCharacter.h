// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "VoiceChatCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEntersCommsRange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLeavesCommsRange);


UCLASS(config=Game)
class AVoiceChatCharacter : public ACharacter
{
	GENERATED_BODY()


public:
#pragma region CommsRange

		UPROPERTY(EditDefaultsOnly, Category = "Voice Chat")
		USphereComponent* RangeComp;

		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Voice Chat")
		float Range = 200;

		UPROPERTY(BlueprintAssignable)
		FOnPlayerEntersCommsRange OnPlayerEnterCommsRange;
		UPROPERTY(BlueprintAssignable)
		FOnPlayerLeavesCommsRange OnPlayerLeaveCommsRange;

		//Bound to BeginOverlap
		UFUNCTION()
		void AddPlayerToComms(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		//Bound to EndOverlap
		UFUNCTION()
		void RemovePlayerFromComms(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
		UFUNCTION(Server, Reliable, WithValidation)
		void Server_GetControllerInVoiceCommRange(AActor* OverlappedActor);
		void Server_GetControllerInVoiceCommRange_Implementation(AActor* OverlappedActor);
		bool Server_GetControllerInVoiceCommRange_Validate(AActor* OverlappedActor);



#pragma endregion








	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	AVoiceChatCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

