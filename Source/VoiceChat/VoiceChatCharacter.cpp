// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "VoiceChat.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "VoiceChatCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AVoiceChatCharacter

AVoiceChatCharacter::AVoiceChatCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	RangeComp = CreateDefaultSubobject<USphereComponent>(TEXT("Range Component"));
	RangeComp->SetSphereRadius(Range);
	RangeComp->SetupAttachment(RootComponent);
	RangeComp->bGenerateOverlapEvents = true;

	RangeComp->OnComponentBeginOverlap.AddDynamic(this, &AVoiceChatCharacter::AddPlayerToComms);
	RangeComp->OnComponentEndOverlap.AddDynamic(this, &AVoiceChatCharacter::RemovePlayerFromComms);




}

//////////////////////////////////////////////////////////////////////////
// Input

void AVoiceChatCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVoiceChatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVoiceChatCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AVoiceChatCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AVoiceChatCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AVoiceChatCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AVoiceChatCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AVoiceChatCharacter::OnResetVR);
}


void AVoiceChatCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVoiceChatCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AVoiceChatCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AVoiceChatCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AVoiceChatCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AVoiceChatCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AVoiceChatCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

/*

Voice Chat

*/

void AVoiceChatCharacter::AddPlayerToComms(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVoiceChatCharacter* OverlappedActor = Cast<AVoiceChatCharacter>(OtherActor);
	if (OverlappedActor)
	{
		Server_GetControllerInVoiceCommRange(OtherActor);
	}
}


void AVoiceChatCharacter::RemovePlayerFromComms(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::MakeRandomColor(), "Actor End Overlapped");
}

void AVoiceChatCharacter::Server_GetControllerInVoiceCommRange_Implementation(AActor* OverlappedActor)
{
	ACharacter* OverlappedChar = Cast<ACharacter>(OverlappedActor);
	if (OverlappedActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "Overlapped with character, getting controller serverside");

		APlayerController* OverlappedController = Cast<APlayerController>(OverlappedChar->GetController());
		if (OverlappedController)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, OverlappedController->PlayerState->PlayerName);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Could not get overlapped controller");
		}
	}
}

bool AVoiceChatCharacter::Server_GetControllerInVoiceCommRange_Validate(AActor* OverlappedActor)
{
	return true;
}
