#include "SpawnerManager.h"

#include "Asteroid.h"
#include "ShipPawn.h"
#include "Engine/World.h"
#include "TimerManager.h"

ASpawnerManager::ASpawnerManager()
{
	PrimaryActorTick.bCanEverTick = false;
	AsteroidClass = AAsteroid::StaticClass();
}

void ASpawnerManager::SetTargetShip(AShipPawn* InTargetShip)
{
	TargetShip = InTargetShip;
}

void ASpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	ScheduleNextSpawn();
}

void ASpawnerManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void ASpawnerManager::ScheduleNextSpawn()
{
	const float Interval = FMath::FRandRange(FMath::Min(MinSpawnInterval, MaxSpawnInterval),
		FMath::Max(MinSpawnInterval, MaxSpawnInterval));
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawnerManager::SpawnAsteroid, Interval, false);
}

void ASpawnerManager::SpawnAsteroid()
{
	if (!AsteroidClass || !GetWorld())
	{
		return;
	}

	const int32 Edge = FMath::RandRange(0, 3);
	FVector SpawnLocation = FVector::ZeroVector;
	switch (Edge)
	{
	case 0:
		SpawnLocation = FVector(-SpawnBounds.X, FMath::FRandRange(-SpawnBounds.Y, SpawnBounds.Y), 0.0f);
		break;
	case 1:
		SpawnLocation = FVector(SpawnBounds.X, FMath::FRandRange(-SpawnBounds.Y, SpawnBounds.Y), 0.0f);
		break;
	case 2:
		SpawnLocation = FVector(FMath::FRandRange(-SpawnBounds.X, SpawnBounds.X), -SpawnBounds.Y, 0.0f);
		break;
	default:
		SpawnLocation = FVector(FMath::FRandRange(-SpawnBounds.X, SpawnBounds.X), SpawnBounds.Y, 0.0f);
		break;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AAsteroid* Asteroid = GetWorld()->SpawnActor<AAsteroid>(AsteroidClass,
		SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
	if (Asteroid)
	{
		const FVector TargetLocation = IsValid(TargetShip) ? TargetShip->GetActorLocation() : FVector::ZeroVector;
		Asteroid->SetFlightDirection(TargetLocation - SpawnLocation);
	}

	ScheduleNextSpawn();
}