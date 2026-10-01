#include "ShipPawn.h"

#include "Asteroid.h"
#include "Projectile.h"
#include "SpaceShooterGameMode.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AShipPawn::AShipPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(CollisionComponent);
	CollisionComponent->SetSphereRadius(55.0f);
	CollisionComponent->SetCollisionObjectType(ECC_Pawn);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AShipPawn::HandleOverlap);

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(CollisionComponent);
	ShipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		ShipMesh->SetStaticMesh(SphereMesh.Object);
		ShipMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.35f));
	}

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CollisionComponent);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector::UpVector);
	MovementComponent->bSnapToPlaneAtStart = true;

	ProjectileClass = AProjectile::StaticClass();
}

void AShipPawn::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent->MaxSpeed = MoveSpeed;
}

void AShipPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector Location = GetActorLocation();
	Location.X = FMath::Clamp(Location.X, -MovementBounds.X, MovementBounds.X);
	Location.Y = FMath::Clamp(Location.Y, -MovementBounds.Y, MovementBounds.Y);
	Location.Z = 0.0f;
	SetActorLocation(Location);
}

void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveHorizontal"), this, &AShipPawn::MoveHorizontal);
	PlayerInputComponent->BindAxis(TEXT("MoveVertical"), this, &AShipPawn::MoveVertical);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShipPawn::Fire);
}

void AShipPawn::MoveHorizontal(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
}

void AShipPawn::MoveVertical(float Value)
{
	AddMovementInput(FVector::RightVector, Value);
}

void AShipPawn::Fire()
{
	if (!ProjectileClass || !GetWorld())
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime < FireInterval)
	{
		return;
	}
	LastFireTime = CurrentTime;

	const FTransform SpawnTransform = ProjectileSpawnPoint->GetComponentTransform();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnTransform, SpawnParameters);

	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleEffect, SpawnTransform);
	}
}

void AShipPawn::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (AAsteroid* Asteroid = Cast<AAsteroid>(OtherActor))
	{
		if (ASpaceShooterGameMode* GameMode = GetWorld()->GetAuthGameMode<ASpaceShooterGameMode>())
		{
			GameMode->HandlePlayerCollision(Asteroid);
		}
	}
}