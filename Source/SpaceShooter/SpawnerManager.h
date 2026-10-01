#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerManager.generated.h"

class AAsteroid;
class AShipPawn;
class UStaticMeshComponent;

UCLASS()
class SPACESHOOTER_API ASpawnerManager : public AActor
{
	GENERATED_BODY()

public:
	ASpawnerManager();
	void SetTargetShip(AShipPawn* InTargetShip);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AAsteroid> AsteroidClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MinSpawnInterval = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float MaxSpawnInterval = 1.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FVector2D SpawnBounds = FVector2D(1350.0f, 800.0f);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void SpawnAsteroid();
	void ScheduleNextSpawn();

	UPROPERTY()
	TObjectPtr<AShipPawn> TargetShip;

	FTimerHandle SpawnTimerHandle;
};