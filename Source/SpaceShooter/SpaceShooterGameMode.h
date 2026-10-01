#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceShooterGameMode.generated.h"

class AAsteroid;
class AShipPawn;
class ASpawnerManager;
class ASpaceShooterPlayerController;

UCLASS()
class SPACESHOOTER_API ASpaceShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpaceShooterGameMode();

	virtual void StartPlay() override;
	void StartGame();
	void RegisterAsteroidDestroyed(int32 ScoreValue);
	void HandlePlayerCollision(AAsteroid* Asteroid);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnStatsChanged(int32 NewScore, int32 NewLives);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnGameOver();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<AShipPawn> ShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<ASpawnerManager> SpawnerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Lives")
	int32 StartingLives = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Game|Score")
	int32 CurrentScore = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Game|Lives")
	int32 RemainingLives = 3;

private:
	UPROPERTY()
	TObjectPtr<ASpawnerManager> ActiveSpawner;

	bool bGameOver = false;
	bool bGameStarted = false;
};