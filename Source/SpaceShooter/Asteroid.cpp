#include "Asteroid.h"

#include "SpaceShooterGameMode.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetSphereRadius(75.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	AsteroidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AsteroidMesh"));
	AsteroidMesh->SetupAttachment(CollisionComponent);
	AsteroidMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		AsteroidMesh->SetStaticMesh(SphereMesh.Object);
		AsteroidMesh->SetRelativeScale3D(FVector(1.5f, 1.1f, 0.8f));
	}
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	HitPoints = FMath::RandRange(FMath::Min(MinHitPoints, MaxHitPoints),
		FMath::Max(MinHitPoints, MaxHitPoints));
	RotationDirection = FMath::RandBool() ? 1.0f : -1.0f;
	SetLifeSpan(25.0f);
}

void AAsteroid::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!FlightDirection.IsNearlyZero())
	{
		AddActorWorldOffset(FlightDirection * FlightSpeed * DeltaSeconds, true);
		AddActorLocalRotation(FRotator(0.0f, 0.0f,
			RotationDirection * RotationSpeed * DeltaSeconds));
	}
}

void AAsteroid::SetFlightDirection(const FVector& Direction)
{
	FlightDirection = Direction.GetSafeNormal();
}

void AAsteroid::ApplyHit(int32 Damage)
{
	HitPoints -= FMath::Max(Damage, 0);
	if (HitPoints > 0)
	{
		return;
	}

	if (DestructionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructionEffect, GetActorTransform());
	}
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
	if (ASpaceShooterGameMode* GameMode = GetWorld()->GetAuthGameMode<ASpaceShooterGameMode>())
	{
		GameMode->RegisterAsteroidDestroyed(ScoreValue);
	}
	Destroy();
}