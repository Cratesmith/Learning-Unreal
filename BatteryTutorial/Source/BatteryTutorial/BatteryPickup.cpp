

#include "BatteryTutorial.h"
#include "BatteryPickup.h"


ABatteryPickup::ABatteryPickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	PowerLevel = 150;
}

void ABatteryPickup::OnPickedUp_Implementation()
{
	Super::OnPickedUp_Implementation();
	Destroy();
}


