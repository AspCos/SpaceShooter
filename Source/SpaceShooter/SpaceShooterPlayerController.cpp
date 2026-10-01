#include "SpaceShooterPlayerController.h"

#include "SpaceShooterGameMode.h"
#include "SpaceShooterHUDWidget.h"
#include "Blueprint/UserWidget.h"

ASpaceShooterPlayerController::ASpaceShooterPlayerController()
{
	bShowMouseCursor = false;
}

void ASpaceShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	HUDWidget = CreateWidget<USpaceShooterHUDWidget>(this, USpaceShooterHUDWidget::StaticClass());
	if (HUDWidget)
	{
		HUDWidget->SetIsFocusable(true);
		HUDWidget->AddToViewport();
		HUDWidget->ShowMainMenu();
		bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(HUDWidget->TakeWidget());
		SetInputMode(InputMode);
	}
}

void ASpaceShooterPlayerController::StartGame()
{
	if (ASpaceShooterGameMode* GameMode = GetWorld()->GetAuthGameMode<ASpaceShooterGameMode>())
	{
		GameMode->StartGame();
	}
	if (HUDWidget)
	{
		HUDWidget->ShowGameplay();
	}
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void ASpaceShooterPlayerController::UpdateStats(int32 NewScore, int32 NewLives)
{
	if (HUDWidget)
	{
		HUDWidget->UpdateStats(NewScore, NewLives);
	}
}

void ASpaceShooterPlayerController::ShowGameOver()
{
	if (HUDWidget)
	{
		HUDWidget->ShowGameOver();
	}
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());
}