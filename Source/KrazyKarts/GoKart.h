// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

private:
	FVector Velocity;
	float Throttle;
	float SteeringThrow;

	// 1 ton - Mass of the car
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// Force applied when throttle is fully down (N)
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	// Minimum radius of the car turning at full lock
	UPROPERTY(EditAnywhere)
	float MinimumTurningRadius = 10;

	// Air drag coefficient
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	// Rolling resistance coefficient
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;


public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ApplyRotation(float DeltaTime);
	void UpdateLocationFromVelocity(float DeltaTime);
	FVector GetAirResistance();
	FVector GetRollingResistance();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveForward(float Val);

	void MoveRight(float Val);


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float Val);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float Val);

};

