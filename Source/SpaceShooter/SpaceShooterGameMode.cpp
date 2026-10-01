#include "SpaceShooterGameMode.h"

#include "Asteroid.h"
#include "ShipPawn.h"
#include "SpawnerManager.h"
#include "SpaceShooterPlayerController.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

ASpaceShooterGameMode::ASpaceShooterGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ASpaceShooterPlayerController::StaticClass();
	ShipClass = AShipPawn::StaticClass();
	SpawnerClass = ASpawnerManager::StaticClass();
}

void ASpaceShooterGameMode::StartPlay()
{
	Super::StartPlay();
	CurrentScore = 0;
	RemainingLives = StartingLives;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	ACameraActor* Camera = GetWorld()->SpawnActor<ACameraActor>(
		FVector(0.0f, 0.0f, 2000.0f), FRotator(-90.0f, 0.0f, 0.0f));
	if (Camera)
	{
		Camera->GetCameraComponent()->SetProjectionMode(ECameraProjectionMode::Orthographic);
		Camera->GetCameraComponent()->OrthoWidth = 2400.0f;
		PlayerController->SetViewTarget(Camera);
	}

	if (ASpaceShooterPlayerController* SpaceShooterController =
		Cast<ASpaceShooterPlayerController>(PlayerController))
	{
		SpaceShooterController->UpdateStats(CurrentScore, RemainingLives);
	}
	OnStatsChanged(CurrentScore, RemainingLives);
}

void ASpaceShooterGameMode::StartGame()
{
	if (bGameStarted || !GetWorld())
	{
		return;
	}
	bGameStarted = true;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	if (ShipClass)
	{
		AShipPawn* Ship = GetWorld()->SpawnActor<AShipPawn>(
			ShipClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (Ship)
		{
			PlayerController->Possess(Ship);
			if (SpawnerClass)
			{
				ActiveSpawner = GetWorld()->SpawnActor<ASpawnerManager>(
					SpawnerClass, FVector::ZeroVector, FRotator::ZeroRotator);
				if (ActiveSpawner)
				{
					ActiveSpawner->SetTargetShip(Ship);
				}
			}
		}
	}
}

void ASpaceShooterGameMode::RegisterAsteroidDestroyed(int32 ScoreValue)
{
	if (bGameOver)
	{
		return;
	}

	CurrentScore += FMath::Max(ScoreValue, 0);
	if (ASpaceShooterPlayerController* PlayerController =
		Cast<ASpaceShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->UpdateStats(CurrentScore, RemainingLives);
	}
	OnStatsChanged(CurrentScore, RemainingLives);
}

void ASpaceShooterGameMode::HandlePlayerCollision(AAsteroid* Asteroid)
{
	if (bGameOver)
	{
		return;
	}

	if (IsValid(Asteroid))
	{
		Asteroid->Destroy();
	}
	RemainingLives = FMath::Max(RemainingLives - 1, 0);
	if (ASpaceShooterPlayerController* PlayerController =
		Cast<ASpaceShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerController->UpdateStats(CurrentScore, RemainingLives);
	}
	OnStatsChanged(CurrentScore, RemainingLives);

	if (RemainingLives == 0)
	{
		bGameOver = true;
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (APawn* PlayerPawn = PlayerController->GetPawn())
			{
				PlayerPawn->DisableInput(PlayerController);
			}
		}
		if (ASpaceShooterPlayerController* PlayerController =
			Cast<ASpaceShooterPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			PlayerController->ShowGameOver();
		}
		if (IsValid(ActiveSpawner))
		{
			ActiveSpawner->Destroy();
		}
		OnGameOver();
	}
}