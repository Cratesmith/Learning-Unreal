

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class ABatteryPickup : public APickup
{
	GENERATED_UCLASS_BODY()

	/** The amount of power in the battery */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Power)
	float PowerLevel;

	/** Overrides OnPickedUp from APickup */
	void OnPickedUp_Implementation() OVERRIDE;
};
