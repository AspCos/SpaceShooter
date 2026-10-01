#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

class UParticleSystem;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SPACESHOOTER_API AAsteroid : public AActor
{
	GENERATED_BODY()

public:
	AAsteroid();

	virtual void Tick(float DeltaSeconds) override;
	void SetFlightDirection(const FVector& Direction);
	void ApplyHit(int32 Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid|Health")
	int32 MinHitPoints = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid|Health")
	int32 MaxHitPoints = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid|Health")
	int32 HitPoints = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid|Movement")
	float FlightSpeed = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid|Score")
	int32 ScoreValue = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asteroid|Effects")
	UParticleSystem* DestructionEffect = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* AsteroidMesh;

	FVector FlightDirection = FVector::ZeroVector;
};