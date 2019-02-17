// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"

FString GetEnumText(ENetRole netRole)
{
	switch (netRole)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "";
	}
}

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;
	Velocity += Acceleration * DeltaTime;

	ApplyRotation(DeltaTime);
	UpdateLocationFromVelocity(DeltaTime);

	DrawDebugString(GetWorld(), FVector(0, 1, 0), GetEnumText(Role), this, FColor::White, DeltaTime);
}

void AGoKart::ApplyRotation(float DeltaTime)
{
	float deltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	float angle = deltaLocation / MinimumTurningRadius * SteeringThrow;
	FQuat rot(GetActorUpVector(), angle);
	AddActorWorldRotation(rot);

	Velocity = rot.RotateVector(Velocity);
}

void AGoKart::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = (Velocity * DeltaTime) * 100.0f;
	FHitResult hitResult;
	AddActorWorldOffset(Translation, true, &hitResult);

	if (hitResult.IsValidBlockingHit()) {
		Velocity = FVector::ZeroVector;
	}
}

FVector AGoKart::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector AGoKart::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
}

void AGoKart::MoveForward(float Val)
{
	Throttle = Val;
	Server_MoveForward(Val);
}

void AGoKart::MoveRight(float Val)
{
	SteeringThrow = Val;
	Server_MoveRight(Val);
}

bool AGoKart::Server_MoveForward_Validate(float Val)
{
	return FMath::Abs(Val) <= 1;
}

void AGoKart::Server_MoveForward_Implementation(float Val)
{
	Throttle = Val;
}

bool AGoKart::Server_MoveRight_Validate(float Val)
{
	return FMath::Abs(Val) <= 1;
}

void AGoKart::Server_MoveRight_Implementation(float Val)
{
	SteeringThrow = Val;
}
