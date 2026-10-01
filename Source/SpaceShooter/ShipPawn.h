#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShipPawn.generated.h"

class AProjectile;
class UParticleSystem;
class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;
class UPrimitiveComponent;
class UInputComponent;

UCLASS()
class SPACESHOOTER_API AShipPawn : public APawn
{
	GENERATED_BODY()

public:
	AShipPawn();

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Ship")
	void MoveHorizontal(float Value);

	UFUNCTION(BlueprintCallable, Category = "Ship")
	void MoveVertical(float Value);

	UFUNCTION(BlueprintCallable, Category = "Ship")
	void Fire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship|Movement")
	float MoveSpeed = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship|Movement")
	FVector2D MovementBounds = FVector2D(1050.0f, 550.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship|Weapon")
	float FireInterval = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship|Weapon")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ship|Weapon")
	UParticleSystem* MuzzleEffect = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UFloatingPawnMovement* MovementComponent;

	float LastFireTime = -1.0e9f;
};