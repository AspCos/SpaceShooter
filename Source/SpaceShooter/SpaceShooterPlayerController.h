#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpaceShooterPlayerController.generated.h"

class USpaceShooterHUDWidget;

UCLASS()
class SPACESHOOTER_API ASpaceShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASpaceShooterPlayerController();

	virtual void BeginPlay() override;
	void StartGame();
	void UpdateStats(int32 NewScore, int32 NewLives);
	void ShowGameOver();

private:
	UPROPERTY()
	TObjectPtr<USpaceShooterHUDWidget> HUDWidget;
};