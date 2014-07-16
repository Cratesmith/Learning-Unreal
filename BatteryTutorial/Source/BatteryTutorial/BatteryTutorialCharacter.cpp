// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "BatteryTutorial.h"
#include "BatteryTutorialCharacter.h"
#include "BatteryPickup.h"
#include "UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "BatteryTutorialGameMode.h"

//////////////////////////////////////////////////////////////////////////
// ABatteryTutorialCharacter

ABatteryTutorialCharacter::ABatteryTutorialCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CharacterMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	CharacterMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	CharacterMovement->JumpZVelocity = 600.f;
	CharacterMovement->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUseControllerViewRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUseControllerViewRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Set default powerlevel and speed values
	PowerLevel		= 2000.0f;
	SpeedFactor		= 0.75f;
	BaseSpeed		= 10.0f;

	// Create and attach the collection sphere to the root
	CollectionSphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollectionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(200.0f);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABatteryTutorialCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("CollectPickups", IE_Pressed, this, &ABatteryTutorialCharacter::CollectBatteries);

	InputComponent->BindAxis("MoveForward", this, &ABatteryTutorialCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABatteryTutorialCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ABatteryTutorialCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ABatteryTutorialCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABatteryTutorialCharacter::TouchStarted);
}


void ABatteryTutorialCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ABatteryTutorialCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryTutorialCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryTutorialCharacter::MoveForward(float Value)
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

void ABatteryTutorialCharacter::MoveRight(float Value)
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

void ABatteryTutorialCharacter::CollectBatteries()
{
	// only collect batteries while playing (Seriously, this doesn't belong in character. Bad tutorial! No biscuit!)
	ABatteryTutorialGameMode* MyGameMode = Cast<ABatteryTutorialGameMode>(UGameplayStatics::GetGameMode(this));
	if (MyGameMode->GetPlayState() != EPlaying)
		return;

	float totalPower = 0.0f;

	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);
	for (int i = 0; i < CollectedActors.Num(); ++i)
	{
		ABatteryPickup* const Battery = Cast<ABatteryPickup>(CollectedActors[i]);
		if (!Battery || Battery->IsPendingKill() || !Battery->bCanBePickedUp)
		{
			continue;
		}

		totalPower += Battery->PowerLevel;
		Battery->OnPickedUp();
		Battery->bCanBePickedUp = false;
	}

	if (totalPower > 0.0f)
	{
		PowerUp(totalPower);
	}
}

void ABatteryTutorialCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CharacterMovement->MaxWalkSpeed = BaseSpeed + PowerLevel*SpeedFactor;
}
