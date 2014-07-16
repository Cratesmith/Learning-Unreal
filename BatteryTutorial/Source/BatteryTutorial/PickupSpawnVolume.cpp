

#include "BatteryTutorial.h"
#include "PickupSpawnVolume.h"
#include "Pickup.h"

APickupSpawnVolume::APickupSpawnVolume(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	WhereToSpawn = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	SpawnDelayRangeMin = 1.0f;
	SpawnDelayRangeMax = 4.5f;

	bSpawningEnabled = true;

	SpawnTime = CalcRandomSpawnDelay();

	PrimaryActorTick.bCanEverTick = true;
}

void APickupSpawnVolume::SpawnPickup()
{
	if (WhatToSpawn == NULL)
		return;

	// Are we actually playing? UWorld is null if we're not playing at the moment
	UWorld* const World = GetWorld();
	if (World == NULL)
		return;

	// Spawn params
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	// Where to spawn
	FVector SpawnLocation = GetRandomPointInVolume();

	// Get a random rotation
	FRotator SpawnRotation(	FMath::Rand()*360.0f,
							FMath::Rand()*360.0f,
							FMath::Rand()*360.0f );

	APickup* const SpawnedPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
}

float APickupSpawnVolume::CalcRandomSpawnDelay() const
{
	return FMath::FRandRange(SpawnDelayRangeMin, SpawnDelayRangeMax);
}

FVector APickupSpawnVolume::GetRandomPointInVolume()
{
	FVector Origin = WhereToSpawn->Bounds.Origin;
	FVector Extents = WhereToSpawn->Bounds.BoxExtent;

	FVector Min = Origin - Extents*0.5f;
	FVector Max = Origin + Extents*0.5f;

	FVector RandomLocation = FVector(FMath::RandRange(Min.X, Max.X),
								FMath::RandRange(Min.Y, Max.Y),
								FMath::RandRange(Min.Z, Max.Z)	);
	return RandomLocation;
}

void APickupSpawnVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SpawnTime -= DeltaSeconds;
	if (SpawnTime > 0.0f)
		return;

	if (bSpawningEnabled == false)
		return;

	SpawnPickup();
	SpawnTime = CalcRandomSpawnDelay();
}

