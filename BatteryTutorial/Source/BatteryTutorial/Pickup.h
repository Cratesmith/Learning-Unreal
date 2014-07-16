

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class APickup : public AActor
{
	GENERATED_UCLASS_BODY()

	/** true when the property is able to be picked up. False disables the pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	bool bCanBePickedUp;
	
	/** collision to use as root component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	TSubobjectPtr<USphereComponent> BaseCollisionComponent;

	/** visual mesh */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	TSubobjectPtr<UStaticMeshComponent> PickupMesh;
	
	/** function to call when collected */
	UFUNCTION(BlueprintNativeEvent)
	void OnPickedUp();	
};
