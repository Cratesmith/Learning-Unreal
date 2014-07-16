

#pragma once

#include "GameFramework/Actor.h"
#include "PickupSpawnVolume.generated.h"

/**
 * 
 */
UCLASS()
class APickupSpawnVolume : public AActor
{
	GENERATED_UCLASS_BODY()

	/** Region in which spawning occurs randomly */
	UPROPERTY(VisibleInstanceOnly, Category = Spawning)
	TSubobjectPtr<UBoxComponent> WhereToSpawn;

	/** The object to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawning)
	TSubclassOf<class APickup> WhatToSpawn;

	/** spawn delay range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawning)
	float SpawnDelayRangeMin;

	/** spawn delay range */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawning)
	float SpawnDelayRangeMax;

	UFUNCTION(BlueprintPure, Category = Spawning)
	FVector GetRandomPointInVolume();

	virtual void Tick(float DeltaSeconds) OVERRIDE;

	UFUNCTION(BlueprintCallable, Category = Spawning)
	bool GetSpawningEnabled() const;
	
	UFUNCTION(BlueprintCallable, Category = Spawning)
	void SetSpawningEnabled(bool val);

private:
	/** random in range min-max spawn range */
	float CalcRandomSpawnDelay() const;

	/** spawn a pickup instance */
	void SpawnPickup();

	/** Countdown for when we should spawn a new pickup */
	float SpawnTime;

	/** is spawning of pickups enabled */
	bool bSpawningEnabled;
};

FORCEINLINE bool APickupSpawnVolume::GetSpawningEnabled() const
{
	return bSpawningEnabled;
}

FORCEINLINE void APickupSpawnVolume::SetSpawningEnabled(bool val)
{
	bSpawningEnabled = val;
}
