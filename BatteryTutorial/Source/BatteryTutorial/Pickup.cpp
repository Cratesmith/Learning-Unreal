

#include "BatteryTutorial.h"
#include "Pickup.h"


APickup::APickup(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// start able to be picked up
	bCanBePickedUp = true;

	// create the collision as root
	BaseCollisionComponent = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponent"));
	RootComponent = BaseCollisionComponent;

	// create the static mesh component
	PickupMesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PickupMesh"));

	// enable physics
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->AttachTo(RootComponent);
}

void APickup::OnPickedUp_Implementation()
{
}