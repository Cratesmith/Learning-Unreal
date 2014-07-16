// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "BatteryTutorial.h"
#include "BatteryTutorialGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BatteryTutorialCharacter.h"
#include "PickupSpawnVolume.h"
#include "BatteryTutorialHUD.h"

ABatteryTutorialGameMode::ABatteryTutorialGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/Blueprints/MyCharacter.MyCharacter_C'"));
	if (PlayerPawnBPClass.Object != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Object;
	}

	// defaults
	PowerDecayRate = 1.0f;

	// custom hud
	HUDClass = ABatteryTutorialHUD::StaticClass();
}

void ABatteryTutorialGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ABatteryTutorialCharacter* MyCharacter = Cast<ABatteryTutorialCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (MyCharacter != NULL)
	{
		if (MyCharacter->PowerLevel > 0.05f)
		{
			// Decay player power
			MyCharacter->PowerLevel = FMath::FInterpTo(MyCharacter->PowerLevel, 0.0f, DeltaSeconds, PowerDecayRate);
		}
		else
		{
			SetPlayState(EGameOver);
		}
	}
}

void ABatteryTutorialGameMode::HandleNewState(EBatteryTutorialPlayState NewState)
{
	switch (NewState)
	{
	case EPlaying:
		{
			for (auto SpawnVolume : SpawnVolumeActors)
			{
				SpawnVolume->SetSpawningEnabled(true);
			}

			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController != NULL)
			{
				PlayerController->SetCinematicMode(false, false, false);
			}
		}
		break;
	case EGameOver:
		{
			for (auto SpawnVolume : SpawnVolumeActors)
			{
				SpawnVolume->SetSpawningEnabled(false);
			}

			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController != NULL)
			{
				PlayerController->SetCinematicMode(true, true, true);
			}
		}
		break;
	case EUnknown:
		break;
	default:
		break;
	}
}

void ABatteryTutorialGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickupSpawnVolume::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		auto SpawnVolume = Cast<APickupSpawnVolume>(Actor);
		if (SpawnVolume == NULL)
			continue;

		SpawnVolumeActors.Add(SpawnVolume);
	}

	SetPlayState(EPlaying);
}
