// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Flight.generated.h"

UCLASS()
class FLIGHTCPP2_API AFlight : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFlight();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Flight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	float mouseX;
	float mouseY;
	float yawSpeed;
	float pitchSpeed;
	float engineSpeed;
	float gCounter = 10.f;
	bool isSleep;
	float sleepCounter = 10.f;
	float isBoost;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float yaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float pitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString warningTxt;

	FVector2D viewSize;
	FVector2D viewportSizeCenter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EngineSpeedUp(float value);
	void EngineSpeedDown(float value);
	void Roll(float value);
	void Yaw(float value);
	void Lift(bool value);
	void Boosta(float value);
	void GWarning();
	void Sleep();
	void Pitch(float value);
	bool RayCast();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
