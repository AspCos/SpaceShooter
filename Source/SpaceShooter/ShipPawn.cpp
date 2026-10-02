#include "ShipPawn.h"

#include "Asteroid.h"
#include "Projectile.h"
#include "SpaceShooterGameMode.h"
#include "InputCoreTypes.h"
#include "GameFramework/PlayerController.h"
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

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && bControlsEnabled)
	{
		// Déplacement : ZQSD (X = haut de l'écran, Y = droite de l'écran)
		FVector MoveInput = FVector::ZeroVector;
		if (PlayerController->IsInputKeyDown(EKeys::Z)) { MoveInput.X += 1.0f; }
		if (PlayerController->IsInputKeyDown(EKeys::S)) { MoveInput.X -= 1.0f; }
		if (PlayerController->IsInputKeyDown(EKeys::D)) { MoveInput.Y += 1.0f; }
		if (PlayerController->IsInputKeyDown(EKeys::Q)) { MoveInput.Y -= 1.0f; }
		if (!MoveInput.IsNearlyZero())
		{
			AddMovementInput(MoveInput.GetSafeNormal(), 1.0f);
		}

		// Tir : flèches directionnelles
		FVector FireDirection = FVector::ZeroVector;
		if (PlayerController->IsInputKeyDown(EKeys::Up)) { FireDirection.X += 1.0f; }
		if (PlayerController->IsInputKeyDown(EKeys::Down)) { FireDirection.X -= 1.0f; }
		if (PlayerController->IsInputKeyDown(EKeys::Right)) { FireDirection.Y += 1.0f; }
		if (PlayerController->IsInputKeyDown(EKeys::Left)) { FireDirection.Y -= 1.0f; }
		if (!FireDirection.IsNearlyZero())
		{
			FireInDirection(FireDirection);
		}
	}

	FVector Location = GetActorLocation();
	Location.X = FMath::Clamp(Location.X, -MovementBounds.X, MovementBounds.X);
	Location.Y = FMath::Clamp(Location.Y, -MovementBounds.Y, MovementBounds.Y);
	Location.Z = 0.0f;
	SetActorLocation(Location);
}

void AShipPawn::EnableInput(APlayerController* PlayerController)
{
	bControlsEnabled = true;
	Super::EnableInput(PlayerController);
}

void AShipPawn::DisableInput(APlayerController* PlayerController)
{
	bControlsEnabled = false;
	Super::DisableInput(PlayerController);
}

void AShipPawn::FireInDirection(const FVector& Direction)
{
	if (!ProjectileClass || !GetWorld())
	{
		return;
	}

	FVector FlatDirection = Direction;
	FlatDirection.Z = 0.0f;
	if (FlatDirection.IsNearlyZero())
	{
		return;
	}
	FlatDirection.Normalize();

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastFireTime < FireInterval)
	{
		return;
	}
	LastFireTime = CurrentTime;

	const FTransform SpawnTransform(
		FlatDirection.Rotation(), GetActorLocation() + FlatDirection * MuzzleOffset);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnTransform, SpawnParameters);
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound,
			SpawnTransform.GetLocation(), FireSoundVolume);
	}

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