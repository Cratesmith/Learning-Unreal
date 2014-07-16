// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

class APickupSpawnVolume;
#include "BatteryTutorialGameMode.generated.h"

enum EBatteryTutorialPlayState : short
{
	EPlaying,
	EGameOver,
	EUnknown
};

UCLASS(minimalapi)
class ABatteryTutorialGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	/** How quickly power decays */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Power)
	float PowerDecayRate;

	virtual void Tick(float DeltaSeconds) OVERRIDE;
	virtual void BeginPlay() OVERRIDE;

	EBatteryTutorialPlayState GetPlayState() const;
	void SetPlayState(EBatteryTutorialPlayState NewState);

private:
	EBatteryTutorialPlayState PlayState;
	void HandleNewState(EBatteryTutorialPlayState NewState);
	TArray<APickupSpawnVolume*> SpawnVolumeActors;
};

FORCEINLINE void ABatteryTutorialGameMode::SetPlayState(EBatteryTutorialPlayState NewState)
{
	if (PlayState == NewState)
		return;

	PlayState = NewState;
	HandleNewState(NewState);
}

FORCEINLINE EBatteryTutorialPlayState ABatteryTutorialGameMode::GetPlayState() const
{
	return PlayState;
}
