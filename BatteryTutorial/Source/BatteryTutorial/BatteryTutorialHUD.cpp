

#include "BatteryTutorial.h"
#include "BatteryTutorialHUD.h"
#include "BatteryTutorialCharacter.h"
#include "BatteryTutorialGameMode.h"


ABatteryTutorialHUD::ABatteryTutorialHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// load the standard font
	static ConstructorHelpers::FObjectFinder<UFont> HUDFontObj(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = HUDFontObj.Object;
}

void ABatteryTutorialHUD::DrawHUD()
{
	auto ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
	
	Super::DrawHUD();

	// Display Powerlevel of player
	auto Player = Cast<ABatteryTutorialCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	auto Display = FString::Printf(TEXT("%10.1f"), FMath::Max(0.0f, Player->PowerLevel));
	DrawText(Display, FColor::White, 50, 50, HUDFont);

	auto GameMode = Cast<ABatteryTutorialGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode->GetPlayState() == EGameOver)
	{
		FVector2D GameOverSize;
		FString GameOverDisplay = TEXT("GAME OVER");
		GetTextSize(GameOverDisplay, GameOverSize.X, GameOverSize.Y, HUDFont);
		DrawText(GameOverDisplay, FColor::White, (ScreenDimensions.X-GameOverSize.X)/2.0f, (ScreenDimensions.X-GameOverSize.Y)/2.0f, HUDFont);
	}
}


