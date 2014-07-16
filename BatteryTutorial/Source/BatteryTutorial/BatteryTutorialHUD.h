

#pragma once

#include "GameFramework/HUD.h"
#include "BatteryTutorialHUD.generated.h"

/**
 * 
 */
UCLASS()
class ABatteryTutorialHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	/** Font for drawing the HUD */
	UPROPERTY()
	UFont* HUDFont;

	virtual void DrawHUD() OVERRIDE;
};
